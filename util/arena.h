// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_UTIL_ARENA_H_
#define STORAGE_LEVELDB_UTIL_ARENA_H_

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

//arena的三种内存分配策略
//1. bytes < 当前块剩余内存 => 直接在当前块分配。
//2. 当前块剩余内存 < bytes < 1024 KB (默认内存块大小的 1 / 4) =>
//   直接申请一个默认大小为 4096 KB 的内存块，然后分配内存。
//3. 当前块剩余内存 < bytes && bytes > 1024 KB => 直接申请一个新的大小为 bytes 的内存块，并分配内存。

namespace leveldb {

//轻量级内存池对象，包括申请内存，分配内存，释放内存。
//申请内存：使用 new 来向操作系统申请一块连续的内存区域。
//分配内存：使用allocate将已经申请的内存分配给项目组件使用，
//这体现在增加 alloc_ptr_ 和减少 alloc_bytes_remaining_ 这两个指针上。
class Arena {
 public:
  Arena();

  Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;

  ~Arena();

  // Return a pointer to a newly allocated memory block of "bytes" bytes.
  // 基本的内存分配函数
  char* Allocate(size_t bytes);

  // Allocate memory with the normal alignment guarantees provided by malloc.
  // 按照字节对齐来分配内存
  char* AllocateAligned(size_t bytes);

  // Returns an estimate of the total memory usage of data allocated
  // by the arena.
  // 返回目前分配的总的内存
  size_t MemoryUsage() const {
    return memory_usage_.load(std::memory_order_relaxed);
  }

 private:
  char* AllocateFallback(size_t bytes);
  char* AllocateNewBlock(size_t block_bytes);

  // Allocation state
  // 当前内存块未分配内存的起始地址
  char* alloc_ptr_;
  // 当前内存块剩余的内存
  size_t alloc_bytes_remaining_;

  // Array of new[] allocated memory blocks
  //存储每个内存块的地址
  std::vector<char*> blocks_;

  // Total memory usage of the arena.
  //
  // TODO(costan): This member is accessed via atomics, but the others are
  //               accessed without any locking. Is this OK?
  // 当前 Arena 已经分配的总内存量
  std::atomic<size_t> memory_usage_;
};

inline char* Arena::Allocate(size_t bytes) {
  // The semantics of what to return are a bit messy if we allow
  // 0-byte allocations, so we disallow them here (we don't need
  // them for our internal use).
  assert(bytes > 0);
  // 申请的内存小于剩余的内存，就直接在当前内存块上分配内存
  if (bytes <= alloc_bytes_remaining_) {
    char* result = alloc_ptr_;
    alloc_ptr_ += bytes;
    alloc_bytes_remaining_ -= bytes;
    return result;
  }
  // 申请的内存的大于当前内存块剩余的内存，就用这个函数来重新申请内存
  return AllocateFallback(bytes);
}

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_UTIL_ARENA_H_
