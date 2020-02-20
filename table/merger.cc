// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "table/merger.h"

#include "leveldb/comparator.h"
#include "leveldb/iterator.h"
#include "table/iterator_wrapper.h"

namespace leveldb {

//需要注意的是，这里的Iterator与STL里面的有点不一样。STL里面的iterator分为前向迭代器和后向迭代器。
//两者的++/--或者说std::next()/std::prev()操作是不一样的。
//但是，在这里，Next()操作的时候，就只能是从小往大的key()方向在操作。并且一旦调用了Next()，迭代器的方向就变成了前向迭代器。
//与此同理，Prev()操作的时候，只能是从大往小的方向操作。一旦调用了Prev()，那么迭代器的方向就变成了向后。
//也就是说，在使用的时候，应该如下使用。
// 由大往小的方向迭代
//auto iter = NewMerginIterator();
//iter->SeekToLast();
//iter->Prev();
//iter->Prev();
//....
//那么反之，如果是想由小往大的方向迭代。那么就需要如下设计：
//
// 由大往小的方向迭代
//auto iter = NewMerginIterator();
//iter->SeekToFirst();
//iter->Next();
//iter->Next();
//....


namespace {
class MergingIterator : public Iterator {
 public:
  // 注意，这里是对children进行了深拷贝
  // 所以需要重新生成n个iterator空间
  // 但是需要注意的是:Iterator是不支持复制的
  // 所以这里是通过重新生成IteratorWrapper
  // 包裹原来的Iterator来使用的。
  MergingIterator(const Comparator* comparator, Iterator** children, int n)
      : comparator_(comparator),
        children_(new IteratorWrapper[n]),
        n_(n),
        current_(nullptr),
        direction_(kForward) {
    for (int i = 0; i < n; i++) {
      children_[i].Set(children[i]);
    }
  }

  ~MergingIterator() override { delete[] children_; }

  //如果还没有设置SeekToFirst()/SeekToLast()/Seek()，那么直接使用current_来调用key()/value()就会出错。
  bool Valid() const override { return (current_ != nullptr); }

  void SeekToFirst() override {
    // 这里把所有的iterator都移动到了自己的开头位置
    // 如果合并的是sst文件
    // 那么就是移动到SST文件的开头
    for (int i = 0; i < n_; i++) {
      children_[i].SeekToFirst();
    }
    // 找到最小的，然后把current_设置过去
    FindSmallest();
    direction_ = kForward;
  }

  void SeekToLast() override {
    for (int i = 0; i < n_; i++) {
      children_[i].SeekToLast();
    }
    FindLargest();
    direction_ = kReverse;
  }

  void Seek(const Slice& target) override {
    for (int i = 0; i < n_; i++) {
      children_[i].Seek(target);
    }
    FindSmallest();
    direction_ = kForward;
  }

  //Next()操作总是在找比当前key()要大的那个Iterator
  //1. 如果移动方向是kReverse，那么需要把除current_之外的iterator都seek到比key()大的地方
  //2. 移动current_->Next()
  void Next() override {
    assert(Valid());

    // 这里需要确保所有的children都已经移动到了小于key()的地方！
    // 如果移动方向是forward即前向移动，那么
    // 因为current_已经是最小的children了，所有后面的Iterator的key
    // 肯定比当前key()大。
    // 否则，也就是说，当移动方向不是kForward的时候，需要显式操作non-current_
    // Ensure that all children are positioned after key().
    // If we are moving in the forward direction, it is already
    // true for all of the non-current_ children since current_ is
    // the smallest child and key() == current_->key().  Otherwise,
    // we explicitly position the non-current_ children.
    // next()是只能前向移动，也就是找到一个比key()大的key()
    // 那么，如果移动方向不是前向的，就需要seek(key())
    // 然后再移动到刚好比key()大的地方
    if (direction_ != kForward) {
      for (int i = 0; i < n_; i++) {
        IteratorWrapper* child = &children_[i];
        // 如果当前的iterator不是current_
        // current_不需要seek()
        // 所以这里只移动其他的iterator
        if (child != current_) {
          // 那么把这个iterator移动>= key()的地方
          child->Seek(key());
          // 如果刚好与current_相等
          // 那么移动到下一个key
          if (child->Valid() &&
              comparator_->Compare(key(), child->key()) == 0) {
            child->Next();
          }
        }
      }
      direction_ = kForward;
    }

    current_->Next();
    //找出最小key
    FindSmallest();
  }

  void Prev() override {
    assert(Valid());

    // Ensure that all children are positioned before key().
    // If we are moving in the reverse direction, it is already
    // true for all of the non-current_ children since current_ is
    // the largest child and key() == current_->key().  Otherwise,
    // we explicitly position the non-current_ children.
    if (direction_ != kReverse) {
      // 如果不是反向移动，那么就需要先seek到>=key的位置
      // 注意：seek如果发现有== key的位置
      // 那么会优先停到==key的位置
      // 当移动到key的位置之后
      // 然后再向前移动一下。
      // 与前面next()同理，这里并不需要去调用current_
      // 的seek(),因为current_只需要Prev()一下就可以了。
      for (int i = 0; i < n_; i++) {
        IteratorWrapper* child = &children_[i];
        if (child != current_) {
          child->Seek(key());
          if (child->Valid()) {
            // Child is at first entry >= key().  Step back one to be < key()
            // 如果已经不是valid的iterator
            // 那么直接移动到最后一个iterator
            // 这样做的目的是什么？
            // 如果k路链表某一路已经合并完了。
            // 那么这一路置空应该就可以了
            child->Prev();
          } else {
            // Child has no entries >= key().  Position at last entry.
            // 虽然说是放到了链表的最后，实际上后面在合并的时候，还是容易带进来
            // 比如某一个路链表就是两个元素[1, 100]
            // 其他链表元素的区间都是在[1, ....., 200]
            // 那么[1, 100]如果只有两个元素，很快就在100, 1, 100, 1之间迭代。
            child->SeekToLast();
          }
        }
      }
      direction_ = kReverse;
    }

    current_->Prev();
    FindLargest();
  }

  Slice key() const override {
    assert(Valid());
    return current_->key();
  }

  Slice value() const override {
    assert(Valid());
    return current_->value();
  }

  Status status() const override {
    Status status;
    for (int i = 0; i < n_; i++) {
      status = children_[i].status();
      if (!status.ok()) {
        break;
      }
    }
    return status;
  }

 private:
  // Which direction is the iterator moving?
  enum Direction { kForward, kReverse };

  void FindSmallest();
  void FindLargest();

  // We might want to use a heap in case there are lots of children.
  // For now we use a simple array since we expect a very small number
  // of children in leveldb.
  const Comparator* comparator_;
  IteratorWrapper* children_;
  int n_;
  IteratorWrapper* current_;
  Direction direction_;
};

//寻找key最小的迭代器
void MergingIterator::FindSmallest() {
  IteratorWrapper* smallest = nullptr;
  for (int i = 0; i < n_; i++) {
    IteratorWrapper* child = &children_[i];
    if (child->Valid()) {
      if (smallest == nullptr) {
        smallest = child;
      } else if (comparator_->Compare(child->key(), smallest->key()) < 0) {
        smallest = child;
      }
    }
  }
  current_ = smallest;
}

//寻找key最大的迭代器
void MergingIterator::FindLargest() {
  IteratorWrapper* largest = nullptr;
  for (int i = n_ - 1; i >= 0; i--) {
    IteratorWrapper* child = &children_[i];
    if (child->Valid()) {
      if (largest == nullptr) {
        largest = child;
      } else if (comparator_->Compare(child->key(), largest->key()) > 0) {
        largest = child;
      }
    }
  }
  current_ = largest;
}
}  // namespace

Iterator* NewMergingIterator(const Comparator* comparator, Iterator** children,
                             int n) {
  assert(n >= 0);
  if (n == 0) {
    return NewEmptyIterator();
  } else if (n == 1) {
    return children[0];
  } else {
    return new MergingIterator(comparator, children, n);
  }
}

}  // namespace leveldb
