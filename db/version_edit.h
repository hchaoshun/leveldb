// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_DB_VERSION_EDIT_H_
#define STORAGE_LEVELDB_DB_VERSION_EDIT_H_

#include <set>
#include <utility>
#include <vector>

#include "db/dbformat.h"

namespace leveldb {

class VersionSet;

//一个文件的元信息
struct FileMetaData {
  FileMetaData() : refs(0), allowed_seeks(1 << 30), file_size(0) {}

  int refs; //引用计数记录了被不同的Version引用的个数，保证被引用中的文件不会被删除
  //优化手段 减到0时会执行compaction
  int allowed_seeks;  // Seeks allowed until compaction
  uint64_t number; //文件编号
  uint64_t file_size;    // File size in bytes
  InternalKey smallest;  // Smallest internal key served by table
  InternalKey largest;   // Largest internal key served by table
};

//两个版本之间的差量delta
class VersionEdit {
 public:
  VersionEdit() { Clear(); }
  ~VersionEdit() = default;

  void Clear();

  void SetComparatorName(const Slice& name) {
    has_comparator_ = true;
    comparator_ = name.ToString();
  }
  void SetLogNumber(uint64_t num) {
    has_log_number_ = true;
    log_number_ = num;
  }
  void SetPrevLogNumber(uint64_t num) {
    has_prev_log_number_ = true;
    prev_log_number_ = num;
  }
  void SetNextFile(uint64_t num) {
    has_next_file_number_ = true;
    next_file_number_ = num;
  }
  void SetLastSequence(SequenceNumber seq) {
    has_last_sequence_ = true;
    last_sequence_ = seq;
  }
  void SetCompactPointer(int level, const InternalKey& key) {
    compact_pointers_.push_back(std::make_pair(level, key));
  }

  // Add the specified file at the specified number.
  // REQUIRES: This version has not been saved (see VersionSet::SaveTo)
  // REQUIRES: "smallest" and "largest" are smallest and largest keys in file
  void AddFile(int level, uint64_t file, uint64_t file_size,
               const InternalKey& smallest, const InternalKey& largest) {
    FileMetaData f;
    f.number = file;
    f.file_size = file_size;
    f.smallest = smallest;
    f.largest = largest;
    new_files_.push_back(std::make_pair(level, f));
  }

  // Delete the specified "file" from the specified "level".
  void DeleteFile(int level, uint64_t file) {
    deleted_files_.insert(std::make_pair(level, file));
  }

  void EncodeTo(std::string* dst) const;
  Status DecodeFrom(const Slice& src);

  std::string DebugString() const;

 private:
  friend class VersionSet;

  //定义删除文件集合，<层次，文件编号>
  typedef std::set<std::pair<int, uint64_t>> DeletedFileSet;

  std::string comparator_;//比较器名称
  uint64_t log_number_;//日志文件编号
  uint64_t prev_log_number_;//上一个日志文件编号
  uint64_t next_file_number_;//下一个文件编号
  SequenceNumber last_sequence_;//上一个序列号
  bool has_comparator_;//是否有比较器
  bool has_log_number_;
  bool has_prev_log_number_;
  bool has_next_file_number_;
  bool has_last_sequence_;

  // 在一个level里面，InternalKey是下次开始做compact的开始的key
  // 对于一个level来说，合并的时候的key的指针不是每次都从头开始扫描。
  // 如果每次合并都从这层level的第一个文件扫描到最后一个文件。实际上是
  // 会增加读的压力。所以一种折中就是每次需要合并的时候，记住上次合并完
  // 成后的位置，下次合并的时候从这里开始合并。
  std::vector<std::pair<int, InternalKey>> compact_pointers_;//整合点<层次，InternalKey键>
  DeletedFileSet deleted_files_;//删除文件集合
  std::vector<std::pair<int, FileMetaData>> new_files_;
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_DB_VERSION_EDIT_H_
