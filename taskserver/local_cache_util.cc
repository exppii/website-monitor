//
// Created by 刘优 on 16/6/8.
//

#include "taskserver/local_cache_util.h"

#include <limits>

#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <leveldb/iterator.h>
#include <leveldb/comparator.h>

#include "taskserver/options.h"
#include "common/varint.h"
#include "common/utils.h"


#ifndef NDEBUG

#include <iostream>
#include <cassert>

#endif

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

using std::shared_ptr;
using std::string;
using leveldb::Slice;
using leveldb::WriteBatch;

namespace webmonitor {

namespace taskserver {

const std::string JOBKEY = "j"; //job jobcontent kv pre
const std::string FRESH = "f"; //fresh node job relationship pre
const std::string OLD = "o"; //old node job relationship pre
const std::string RALATED = "r"; //job node relationship pre relate
const std::string TRASH = "t"; //delete job relationship
const size_t LONG_SIZE = 8;

//
//bool in_range_compare(const Slice &a, const Slice &range) {
//#ifndef NDEBUG
//  assert(a.size()>= range.size() );
//  printf("current a: %s, range: %s \n", a.data(), range.data());
//#endif
//  Slice a1(a.data(),range.size());
//  return a1 == range;
//}

class LocalCachedUtil : public LocalCachedUtilInterface {

public:

  LocalCachedUtil(Options *option);

  ~LocalCachedUtil();

  bool store_job(const JobDef &job) override;

  bool update_job(const JobDef &job) override;

  bool del_job(const uint64_t &job_id) override;

  bool get_fresh_task_list(const uint64_t &node_id, TaskMap *tasks) override;

  bool get_whole_task_list(const uint64_t &node_id, TaskMap *tasks) override;

  uint64_t get_count(const uint64_t &node_id) override;

  bool get_delete_task_list(const uint64_t& node_id,  IdList* ids) override;

  uint64_t get_range_count(const std::string &range) override;
#ifndef NDEBUG
  void print_all() override;
#endif
private:

  string _id_to_string(const uint64_t &id) const {
    char buf[LONG_SIZE] = {0};
    encode_fixint64(buf, id);
    return std::string(buf, LONG_SIZE);
  }


  void _init_store_job_batch(const JobDef& job, WriteBatch* batch) const;

  void _init_del_job_batch(const uint64_t& id, WriteBatch* batch);

  size_t _read_task_map(const std::vector<string>& ids, TaskMap *tasks);

  //std::vector<uint64_t> _scan_id_range(const Slice &start, const Slice &end);
  std::vector<string> _scan_value_range(const Slice &range);

  std::vector<string> _scan_key_range(const Slice &range);

  uint64_t _scan_count(const Slice &range);

private:

  leveldb::Options _options;
  ::leveldb::DB *_db;
};

bool LocalCachedUtil::store_job(const JobDef &job) {

  leveldb::WriteBatch batch;
  _init_store_job_batch(job, &batch);
  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LocalCachedUtil::update_job(const JobDef &job) {

  leveldb::WriteBatch batch;
  _init_del_job_batch(job.id(), &batch);
  _init_store_job_batch(job, &batch);

  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LocalCachedUtil::del_job(const uint64_t &job_id) {

  leveldb::WriteBatch batch;

  _init_del_job_batch(job_id, &batch);
  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LocalCachedUtil::get_fresh_task_list(const uint64_t &node_id,
                                          TaskMap *tasks) {
  const std::string n = _id_to_string(node_id);
  auto jobids = _scan_key_range(FRESH + n);

  leveldb::WriteBatch batch;

  for (const auto &j : jobids) {
    batch.Delete(FRESH + n + j);
    batch.Put(OLD + n + j, Slice());
  }

  _read_task_map(jobids, tasks);
  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LocalCachedUtil::get_whole_task_list(const uint64_t &node_id, TaskMap *tasks) {

  const std::string n = _id_to_string(node_id);
  auto fresh_ids = _scan_key_range(FRESH + n);

  leveldb::WriteBatch batch;

  for (const auto &j : fresh_ids) {
    batch.Delete(FRESH + n + j);
    batch.Put(OLD + n + j, Slice());
  }

  auto old_ids = _scan_key_range(OLD + n);

  fresh_ids.insert(fresh_ids.end(),old_ids.cbegin(), old_ids.cend());

  _read_task_map(fresh_ids, tasks);


  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LocalCachedUtil::get_delete_task_list(const uint64_t& node_id, IdList* ids) {

  const std::string n = _id_to_string(node_id);
  leveldb::Slice range(TRASH + n);
  auto jobid_list = _scan_key_range(range);

  leveldb::WriteBatch batch;
  for (const auto &j : jobid_list) {
    ids->Add(decode_fixed64(j.c_str()));
    batch.Delete(TRASH + n + j);
  }

  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

uint64_t LocalCachedUtil::get_count(const uint64_t &node_id) {

  Slice range(OLD + _id_to_string(node_id));
  return _scan_count(range);

}

LocalCachedUtil::LocalCachedUtil(Options *options) {

  _options.create_if_missing = true;
  mkdir_if_not_exists(options->get_cachefile_path());

  leveldb::Status status = leveldb::DB::Open(_options,
                                             options->get_cachefile_path() +
                                             "cacheddb", &_db);
  assert(status.ok());
}

LocalCachedUtil::~LocalCachedUtil() {
  delete _db;
}


void LocalCachedUtil::_init_store_job_batch(const JobDef& job, leveldb::WriteBatch* batch) const {

  const std::string j = _id_to_string(job.id());
  Slice emptyval{};
  //# 第一步 创建 job kv对 插入数据库
  batch->Put(JOBKEY + j, job.task().SerializeAsString());
  //# 第二步 创建 node 和 job reserve关联 kv 对
  for (const auto &node: job.node_list()) {
    const std::string n = _id_to_string(node);
    batch->Put(FRESH + n + j, emptyval);
    batch->Put(RALATED + j + n, emptyval);
  }
}

void LocalCachedUtil::_init_del_job_batch(const uint64_t& id, leveldb::WriteBatch* batch) {

  const std::string j = _id_to_string(id);
  leveldb::Slice range{RALATED + j}, emptyval{};

  auto nodes = _scan_key_range(range);

  for (const auto &n : nodes) {
    batch->Delete(FRESH + n + j);
    batch->Delete(OLD + n + j);
    batch->Delete(RALATED + j + n);
    batch->Put(TRASH + n + j, emptyval); //
  }
  batch->Delete(JOBKEY + j);

}


size_t LocalCachedUtil::_read_task_map(const std::vector<string>& ids, TaskMap *tasks) {
  size_t ret = tasks->size();
  TaskDef t;
  for (const auto &j : ids) {
    std::string val{};
    _db->Get(leveldb::ReadOptions(), JOBKEY + j, &val);
    if (t.ParseFromString(val)) tasks->insert({decode_fixed64(j.c_str()), t});
  }
  return tasks->size() - ret;
}

std::vector<string> LocalCachedUtil::_scan_key_range(const Slice &range) {
  std::vector<string> key_list{};
  const auto LENGTH = range.size();
  shared_ptr<leveldb::Iterator> iter(_db->NewIterator(leveldb::ReadOptions()));

  for (iter->Seek(range); iter->Valid() && iter->key().starts_with(range);
       iter->Next()) {
    std::string val((iter->key().data() + LENGTH), iter->key().size() - LENGTH);
    key_list.push_back(val);
  }
  return key_list;
}

std::vector<string> LocalCachedUtil::_scan_value_range(const Slice &range) {
  std::vector<string> task_list{};

  shared_ptr<leveldb::Iterator> iter(_db->NewIterator(leveldb::ReadOptions()));

  for (iter->Seek(range); iter->Valid() && iter->key().starts_with(range);
       iter->Next()) {
    task_list.push_back(iter->value().ToString());
  }
  return task_list;
}

uint64_t LocalCachedUtil::_scan_count(const Slice &range) {

  uint64_t count = 0;
  shared_ptr<leveldb::Iterator> iter(_db->NewIterator(leveldb::ReadOptions()));

  for (iter->Seek(range);
       iter->Valid() && iter->key().starts_with(range);
       iter->Next()) {
    ++count;
  }
  return count;
}

uint64_t LocalCachedUtil::get_range_count(const std::string &range) {

  return _scan_count(range);
}



#ifndef NDEBUG
void LocalCachedUtil::print_all() {
  shared_ptr<leveldb::Iterator> iter(_db->NewIterator(leveldb::ReadOptions()));

  for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {

    auto key = iter->key();
    std::cout << "iter key size: " << key.size() << "\t";
    if (key.size() == 9) {
      std::cout << key[0] << decode_fixed64(key.data() + 1) << std::endl;
    } else if (key.size() == 17) {
      std::cout << key[0] << decode_fixed64(key.data() + 1) <<
      decode_fixed64(key.data() + 9) << std::endl;
    }

  }
}
#endif


LocalCachedUtilInterface* NewLevelDBCachedUtilPtr(Options *option) {

  return new LocalCachedUtil(option);

}

} //namespace taskserver

} //namspace webmonitor
