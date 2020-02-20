**LevelDB is a fast key-value storage library written at Google that provides an ordered mapping from string keys to string values.**

[![Build Status](https://travis-ci.org/google/leveldb.svg?branch=master)](https://travis-ci.org/google/leveldb)
[![Build status](https://ci.appveyor.com/api/projects/status/g2j5j4rfkda6eyw5/branch/master?svg=true)](https://ci.appveyor.com/project/pwnall/leveldb)

Authors: Sanjay Ghemawat (sanjay@google.com) and Jeff Dean (jeff@google.com)

# Features

  * Keys and values are arbitrary byte arrays.
  * Data is stored sorted by key.
  * Callers can provide a custom comparison function to override the sort order.
  * The basic operations are `Put(key,value)`, `Get(key)`, `Delete(key)`.
  * Multiple changes can be made in one atomic batch.
  * Users can create a transient snapshot to get a consistent view of data.
  * Forward and backward iteration is supported over the data.
  * Data is automatically compressed using the [Snappy compression library](http://google.github.io/snappy/).
  * External activity (file system operations etc.) is relayed through a virtual interface so users can customize the operating system interactions.

# Documentation

  [LevelDB library documentation](https://github.com/google/leveldb/blob/master/doc/index.md) is online and bundled with the source code.


## Repository contents

See [doc/index.md](doc/index.md) for more explanation. See
[doc/impl.md](doc/impl.md) for a brief overview of the implementation.

The public interface is in include/leveldb/*.h.  Callers should not include or
rely on the details of any other header files in this package.  Those
internal APIs may be changed without warning.

Guide to header files:

* **include/leveldb/db.h**: Main interface to the DB: Start here.

* **include/leveldb/options.h**: Control over the behavior of an entire database,
and also control over the behavior of individual reads and writes.

* **include/leveldb/comparator.h**: Abstraction for user-specified comparison function.
If you want just bytewise comparison of keys, you can use the default
comparator, but clients can write their own comparator implementations if they
want custom ordering (e.g. to handle different character encodings, etc.).

* **include/leveldb/iterator.h**: Interface for iterating over data. You can get
an iterator from a DB object.

* **include/leveldb/write_batch.h**: Interface for atomically applying multiple
updates to a database.

* **include/leveldb/slice.h**: A simple module for maintaining a pointer and a
length into some other byte array.

* **include/leveldb/status.h**: Status is returned from many of the public interfaces
and is used to report success and various kinds of errors.

* **include/leveldb/env.h**:
Abstraction of the OS environment.  A posix implementation of this interface is
in util/env_posix.cc.

* **include/leveldb/table.h, include/leveldb/table_builder.h**: Lower-level modules that most
clients probably won't use directly.

# References
- [leveldb中的SSTable (2)](https://bean-li.github.io/leveldb-sstable-index-block/)
- [leveldb(3) 元数据](https://www.jianshu.com/p/f3ebe211e171)
- [LevelDB源码解析18.Block的Iterator](https://zhuanlan.zhihu.com/p/45217164)
- [LevelDB源码解析26. 二级迭代器](https://zhuanlan.zhihu.com/p/45829937)
- [leveldb Arena 分析](https://www.jianshu.com/p/f5eebf44dec9)
- [leveldb中的memtable](https://bean-li.github.io/leveldb-memtable/)
- [leveldb 笔记五：LRUCache的实现](http://kaiyuan.me/2017/06/12/leveldb-05/)
- [leveldb：version分析](https://blog.csdn.net/weixin_36145588/article/details/77984142)
- [LevelDB源码解析10.创建VersionSet](https://zhuanlan.zhihu.com/p/35275467)
- [leveldb源码剖析----compaction](https://blog.csdn.net/Swartz2015/article/details/67633724)
- [leveldb源码剖析--数据写入(DBImpl::Write)](https://blog.csdn.net/Swartz2015/article/details/66970885)
- [leveldb源码剖析---DBImpl::MakeRoomForWrite函数的实现](https://blog.csdn.net/Swartz2015/article/details/66972106)
- [LevelDB 之 Compaction](https://zhuanlan.zhihu.com/p/46718964)
- [NoSQL存储-LSM树](https://juejin.im/post/5bbbf7615188255c59672125)
- [LevelDB源码解析23. Merge Iterator](https://zhuanlan.zhihu.com/p/45661955)