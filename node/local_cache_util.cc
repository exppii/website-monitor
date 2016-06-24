//
// Created by 刘优 on 16/6/21.
//

#include "node/local_cache_util.h"


#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <leveldb/iterator.h>
#include <leveldb/comparator.h>

#include "node/options.h"

#include "common/varint.h"
#include "common/utils.h"

#ifndef NDEBUG

#include <iostream>
#endif

using std::shared_ptr;
using std::string;
using leveldb::Slice;
using leveldb::WriteBatch;

namespace webmonitor {

namespace node {


const std::string DATA_TAG = "d";
const size_t LONG_SIZE = 8;

class LevelDBCached : public LocalCachedInterface {

public:

  explicit LevelDBCached(const Options *);

  ~LevelDBCached();

  //only support single thread
  bool add(const std::string &data) override;

  //only support single thread
  bool get(std::string *data) override;

  //only support single thread
  bool del_last_get() override;

  bool recovery() override;

#ifndef NDEBUG

  uint64_t get_range_count(const std::string &range) override;

  std::string get_end_key() const override {
    return _read_end.size() == 1 ? _read_end : DATA_TAG + std::to_string(
        decode_fixed64(_read_end.data() + 1));
  }

  std::string get_start_key() const override {

    return DATA_TAG + std::to_string(decode_fixed64(_curr_start.data() + 1));
  }


#endif

private:

  string _next_local_key(const Slice &key) const;
  string _prev_local_key(const Slice &key) const;

  string _id_to_string(uint64_t id) const {
    char buf[LONG_SIZE] = {0};
    encode_fixint64(buf, id);
    std::string ret(buf,LONG_SIZE);
    return ret;
  }

private:

  const uint32_t _BATCH_SIZE;

  //store last key in string not slice
  // @see https://github.com/google/leveldb/issues/387

  string _last_start{DATA_TAG};
  string _curr_start{DATA_TAG};
  string _read_end{DATA_TAG};

  uint64_t _local_count = 0;
  leveldb::Options _options;
  ::leveldb::DB *_db;
};

bool LevelDBCached::add(const std::string &data) {
  //pre + timestamp + local_count
  auto s = _db->Put(leveldb::WriteOptions(),
                    DATA_TAG + _id_to_string(++_local_count), data);
  return s.ok();
}

bool LevelDBCached::get(std::string *data) {

  shared_ptr<leveldb::Iterator> iter(_db->NewIterator(leveldb::ReadOptions()));

  uint32_t length = 0;

  Slice end{};

  for (iter->Seek(_curr_start); ++length <= _BATCH_SIZE && iter->Valid(); iter->Next()) {
    end = iter->key();
    (*data) += iter->value().ToString() + "||";
  }

  if( length > 1) {

    _last_start = _curr_start;

    if(length > _BATCH_SIZE && iter->Valid() ) {
      _curr_start = iter->key().ToString();
      _read_end = _prev_local_key(iter->key());

    } else {
      _curr_start = _next_local_key(end);
      _read_end = end.ToString();
    }

    return true;

  }




  return false;
}

bool LevelDBCached::del_last_get() {

  shared_ptr<leveldb::Iterator> iter(_db->NewIterator(leveldb::ReadOptions()));

  leveldb::WriteBatch batch;

  for (iter->Seek(_read_end); iter->Valid(); iter->Prev()) {
    batch.Delete(iter->key());
  }

  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LevelDBCached::recovery() {
  _curr_start = _last_start;
  _read_end = _prev_local_key(_last_start);
  return true;
}

#ifndef NDEBUG

uint64_t LevelDBCached::get_range_count(const std::string &range) {
  uint64_t count = 0;
  shared_ptr<leveldb::Iterator> iter(_db->NewIterator(leveldb::ReadOptions()));

  for (iter->Seek(range);
       iter->Valid() && iter->key().starts_with(range);
       iter->Next()) {
    ++count;
  }
  return count;
}

#endif

LevelDBCached::LevelDBCached(const Options *options)
    : _BATCH_SIZE(options->get_batch_size()) {
  _options.create_if_missing = true;
  mkdir_if_not_exists(options->get_wal_path());

  auto status = leveldb::DB::Open(_options, options->get_wal_path(), &_db);
  assert(status.ok());


}

LevelDBCached::~LevelDBCached() {
  delete _db;
}

string LevelDBCached::_next_local_key(const Slice &key) const {
  auto c = decode_fixed64(key.data() + 1);
  return DATA_TAG + _id_to_string(++c);

}

string LevelDBCached::_prev_local_key(const Slice &key) const {

  auto c = decode_fixed64(key.data() + 1);
  return DATA_TAG + _id_to_string(--c);

}


LocalCachedInterface *NewLevelDBCachedPtr(const Options *options) {
  return new LevelDBCached(options);
}

} //namespace node

} //namespace webmonitor
