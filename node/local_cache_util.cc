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

using std::shared_ptr;
using std::string;
using leveldb::Slice;
using leveldb::WriteBatch;

namespace webmonitor {

namespace node {

static int64_t local_count = 0;
const std::string DATA_TAG = "d";
const std::string END_TAG = "e";
const std::string TAKED_TAG = "t";
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

  std::string get_limit_key() const override {
    return _limit.size() == 1 ? _limit.ToString() : DATA_TAG + std::to_string(
        decode_fixed64(_limit.data() + 1));
  }

#endif

private:

  string _id_to_string(const uint64_t &id) const {
    char buf[LONG_SIZE] = {0};
    encode_fixint64(buf, id);
    return std::string(buf, LONG_SIZE);
  }

private:

  const uint32_t _BATCH_SIZE;
  Slice _limit{DATA_TAG};

  leveldb::Options _options;
  ::leveldb::DB *_db;
};

bool LevelDBCached::add(const std::string &data) {
  //pre + timestamp + local_count
  auto s = _db->Put(leveldb::WriteOptions(),
                    DATA_TAG + _id_to_string(++local_count), data);
  return s.ok();
}

bool LevelDBCached::get(std::string *data) {

  shared_ptr<leveldb::Iterator> iter(_db->NewIterator(leveldb::ReadOptions()));

  uint32_t length = 0;

  for (iter->Seek(_limit);
       iter->Valid() && ++length <= _BATCH_SIZE;
       iter->Next()) {
    (*data) += iter->value().ToString() + "||";
  }

  _limit = (iter->Valid() && length > _BATCH_SIZE) ? iter->key() : Slice(
      END_TAG);

  return length > 0;
}

bool LevelDBCached::del_last_get() {

  shared_ptr<leveldb::Iterator> iter(_db->NewIterator(leveldb::ReadOptions()));

  leveldb::WriteBatch batch;

  if(_limit == Slice(END_TAG)) {
    for (iter->SeekToLast(); iter->Valid(); iter->Prev()) {
      batch.Delete(iter->key());
    }
    _limit = Slice(DATA_TAG);
  } else {
    for (iter->Seek(_limit), iter->Prev(); iter->Valid(); iter->Prev()) {
      batch.Delete(iter->key());
    }
  }
  auto s = _db->Write(leveldb::WriteOptions(), &batch);

  return s.ok();
}

bool LevelDBCached::recovery() {
  _limit = Slice(DATA_TAG);
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
//  _db->Put(leveldb::WriteOptions(),
//                   END_TAG + _id_to_string(++local_count), data);


}

LevelDBCached::~LevelDBCached() {
  delete _db;
}


LocalCachedInterface *NewLevelDBCachedPtr(const Options *options) {
  return new LevelDBCached(options);
}

} //namespace node

} //namespace webmonitor
