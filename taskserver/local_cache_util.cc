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

namespace webmonitor {

namespace taskserver {

const std::string JOBKEY = "j"; //job jobcontent kv pre
const std::string FRESH = "f"; //fresh node job relationship pre
const std::string OLD = "o"; //old node job relationship pre
const std::string RALATED = "r"; //job node relationship pre relate
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

  bool store_task_node_ship(const uint64_t &job_id,
                            const uint64_t &node_id) override;

  uint64_t get_count(const uint64_t &node_id) override;

  uint64_t get_range_count(const std::string &range) override;

  void print_all() override;

private:

  string _id_to_string(const uint64_t &id) {
    char buf[LONG_SIZE] = {0};
    encode_fixint64(buf, id);
    return std::string(buf, LONG_SIZE);
  }

  //std::vector<uint64_t> _scan_id_range(const Slice &start, const Slice &end);
  std::vector<string> _scan_value_range(const Slice &range);

  std::vector<string> _scan_key_range(const Slice &range);

  uint64_t _scan_count(const Slice &range);

private:

  leveldb::Options _options;
  ::leveldb::DB *_db;
};

bool LocalCachedUtil::store_job(const JobDef &job) {

  const std::string jobid = _id_to_string(job.id());

  Slice jkey(JOBKEY + jobid);

  std::string taskstring{};
  job.task().SerializeToString(&taskstring);

  leveldb::WriteBatch batch;
  //# 第一步 创建 job kv对 插入数据库
  batch.Put(jkey, taskstring);
  //# 第二步 创建 node 和 job reserve关联 kv 对
  Slice emptyval{};
  for (const auto &nid: job.node_list()) {
    const std::string nodeid = _id_to_string(nid);
    Slice reserve(FRESH + nodeid + jobid);
    Slice job_related(RALATED + jobid + nodeid);
    batch.Put(reserve, emptyval);
    batch.Put(job_related, emptyval);
  }

  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LocalCachedUtil::update_job(const JobDef &job) {

  const std::string jobid = _id_to_string(job.id());
  leveldb::Slice range(RALATED + jobid);

  //scan all node list related with jobid;
  auto node_list = _scan_key_range(range);

  leveldb::WriteBatch batch;
  //# 第一步 删除有job关联 kv对

  for (const auto &nid : node_list) {
    batch.Delete(FRESH + nid + jobid);
    batch.Delete(OLD + nid + jobid);
    batch.Delete(RALATED + jobid + nid);
  }

  std::string taskstring{};
  job.task().SerializeToString(&taskstring);
  //# 第一步 创建 job kv对 插入数据库
  batch.Put(JOBKEY + jobid, taskstring);
  //# 第二步 创建 node 和 job reserve关联 kv 对
  Slice emptyval{};
  for (const auto &nid: job.node_list()) {
    const std::string nodeid = _id_to_string(nid);
    Slice reserve(FRESH + nodeid + jobid);
    Slice job_related(RALATED + jobid + nodeid);
    batch.Put(reserve, emptyval);
    batch.Put(job_related, emptyval);
  }

  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LocalCachedUtil::del_job(const uint64_t &job_id) {

  const std::string jobid = _id_to_string(job_id);
  leveldb::Slice range(RALATED + jobid);

  //scan all node list related with jobid;
  auto node_list = _scan_key_range(range);

  leveldb::WriteBatch batch;
  //# 第一步 删除有job关联 kv对
  for (const auto &nid : node_list) {
    batch.Delete(FRESH + nid + jobid);
    batch.Delete(OLD + nid + jobid);
    batch.Delete(RALATED + jobid + nid);
  }
  batch.Delete(JOBKEY + jobid);

  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LocalCachedUtil::get_fresh_task_list(const uint64_t &node_id,
                                          TaskMap *tasks) {
  const std::string nodeid = _id_to_string(node_id);
  leveldb::Slice range(FRESH + nodeid);
  auto jobid_list = _scan_key_range(range);
  TaskDef t;
  for (const auto &id : jobid_list) {
    Slice key(JOBKEY + id);
    std::string val;
    _db->Get(leveldb::ReadOptions(), key, &val);
    if (t.ParseFromString(val))
      tasks->insert({decode_fixed64(id.c_str()), t});
  }

  Slice emptyval{};
  leveldb::WriteBatch batch;
  for (const auto &jobid : jobid_list) {
    Slice skey{FRESH + nodeid + jobid};
    Slice rkey{OLD + nodeid + jobid};
    batch.Delete(skey);
    batch.Put(rkey, emptyval);
  }
  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LocalCachedUtil::get_whole_task_list(const uint64_t &node_id,
                                          TaskMap *tasks) {



  get_fresh_task_list(node_id,tasks);

  TaskDef t;
  const std::string nodeid = _id_to_string(node_id);
  leveldb::Slice range2(OLD + nodeid);
  auto jobid2_list = _scan_key_range(range2);
  for (const auto &id : jobid2_list) {
    Slice key(JOBKEY + id);
    std::string val;
    _db->Get(leveldb::ReadOptions(), key, &val);
    if (t.ParseFromString(val))
      tasks->insert({decode_fixed64(id.c_str()), t});
  }

  return true;
}

bool LocalCachedUtil::store_task_node_ship(const uint64_t &job_id,
                                           const uint64_t &node_id) {
  return false;
}

uint64_t LocalCachedUtil::get_count(const uint64_t &node_id) {

  Slice range(OLD + _id_to_string(node_id));
  return _scan_count(range);

}

LocalCachedUtil::LocalCachedUtil(Options *option) {

  _options.create_if_missing = true;
  mkdir_if_not_exists(option->get_cachefile_path());

  leveldb::Status status = leveldb::DB::Open(_options,
                                             option->get_cachefile_path() +
                                             "cacheddb", &_db);
  assert(status.ok());
}

LocalCachedUtil::~LocalCachedUtil() {
  delete _db;
}

//std::vector<uint64_t> LocalCachedUtil::_scan_id_range(const Slice &start,
//                                                      const Slice &end) {
//  std::vector<uint64_t> id_list{};
//  shared_ptr<leveldb::Iterator> iter(_db->NewIterator(leveldb::ReadOptions()));
//
//  for (iter->Seek(start);
//       iter->Valid() && _options.comparator->Compare(iter->key(), end) <= 0;
//       iter->Next()) {
//    auto val = iter->key().data();
//    id_list.push_back(decode_fixed64(val + 9));
//
//  }
//  return id_list;
//}

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







//
//void LocalCachedUtil::_scan_range(const Slice &startkey,
//                                  const const Slice &endkey) {
//
//
//}


std::unique_ptr<LocalCachedUtilInterface> LevelDBCachedUtilUniquePtr(
    Options *option) {

  return make_unique<LocalCachedUtil>(option);

}

} //namespace taskserver

} //namspace webmonitor
