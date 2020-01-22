// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_TABLE_BLOCK_H_
#define STORAGE_LEVELDB_TABLE_BLOCK_H_

#include <stddef.h>
#include <stdint.h>

#include "leveldb/iterator.h"

namespace leveldb {

struct BlockContents;
class Comparator;

//Block存储data block或metaindex block或index block内容,有一个迭代器迭代block里的kv
class Block {
 public:
  // Initialize the block with the specified contents.
  //BlockContents指内存数据区的值
  explicit Block(const BlockContents& contents);

  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;

  ~Block();

  size_t size() const { return size_; }
  Iterator* NewIterator(const Comparator* comparator);

 private:
  //迭代器迭代block里的kv
  class Iter;

  uint32_t NumRestarts() const;

  const char* data_;          // data block 的起始位置
  size_t size_;               // data block 的长度
  uint32_t restart_offset_;  // Offset in data_ of restart array, restarts的开始位置
  bool owned_;               // Block owns data_[]
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_TABLE_BLOCK_H_
