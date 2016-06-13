//
// Created by 刘优 on 16/6/8.
//

#include "taskserver/local_cache_util.h"

#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <leveldb/iterator.h>
#include <leveldb/comparator.h>
#include "common/varint.h"

#ifndef NDEBUG

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
const std::string RESERVE_RALATED = "s"; //reserve node job relationship pre
const std::string REGULAR_RALATED = "r"; //regular node job relationship pre
const std::string JOB_RALATED = "n"; //job node relationship pre relate
const size_t LONG_SIZE = 8;


bool in_range_compare(const Slice &a, const Slice &b) {
#ifndef NDEBUG
  assert(a.size()> LONG_SIZE );
  assert(b.size()> LONG_SIZE);
#endif
  Slice a1(a.data(),LONG_SIZE + 1),b1(b.data(),LONG_SIZE + 1);
  return a1.compare(b1) == 0;
}

class LocalCachedUtil : public LocalCachedUtilInterface {

public:

  LocalCachedUtil(const std::string &path, Options *option);

  ~LocalCachedUtil();

  bool store_job(const JobDef &job) override;

  bool update_job(const JobDef &job) override;

  bool del_job(const uint64_t &job_id) override;

  bool get_fresh_task_list(const uint64_t &node_id, TaskMap *tasks) override;

  bool get_whole_task_list(const uint64_t &node_id, TaskMap *tasks) override;

  bool store_task_node_ship(const uint64_t &job_id,
                            const uint64_t &node_id) override;

  bool get_count(const uint64_t &node_id, uint64_t *count) override;

private:

  string _id_to_string(const uint64_t& id) {
    char buf[LONG_SIZE] = {0};
    encode_fixint64(buf, id);
    return std::string(buf,LONG_SIZE);
  }

  //std::vector<uint64_t> _scan_id_range(const Slice &start, const Slice &end);
  std::vector<string> _scan_range(const Slice &start);

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
  batch.Put(JOBKEY, taskstring);
  //# 第二步 创建 node 和 job reserve关联 kv 对
  for (const auto &nid: job.node_list()) {
    const std::string nodeid = _id_to_string(nid);
    Slice reserve(RESERVE_RALATED + nodeid + jobid);
    Slice job_related(JOB_RALATED + jobid + nodeid);
    batch.Put(reserve, jobid);
    batch.Put(job_related, nodeid);
  }

  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LocalCachedUtil::update_job(const JobDef &job) {

  const std::string jobid = _id_to_string(job.id());
  leveldb::Slice startkey(JOB_RALATED + jobid + _id_to_string(0));

  //scan all node list related with jobid;
  auto node_list = _scan_range(startkey);

  leveldb::WriteBatch batch;
  //# 第一步 删除有job关联 kv对
  for(const auto& nid : node_list) {
    batch.Delete(RESERVE_RALATED + nid + jobid);
    batch.Delete(REGULAR_RALATED + nid + jobid);
    batch.Delete(JOB_RALATED + jobid + nid);
  }

  std::string taskstring{};
  job.task().SerializeToString(&taskstring);
  //# 第一步 创建 job kv对 插入数据库
  batch.Put(JOBKEY + jobid, taskstring);
  //# 第二步 创建 node 和 job reserve关联 kv 对
  for (const auto &nid: job.node_list()) {
    const std::string nodeid = _id_to_string(nid);
    Slice reserve(RESERVE_RALATED + nodeid + jobid);
    Slice job_related(JOB_RALATED + jobid + nodeid);
    batch.Put(reserve, jobid);
    batch.Put(job_related, nodeid);
  }

  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LocalCachedUtil::del_job(const uint64_t &job_id) {

  const std::string jobid = _id_to_string(job_id);
  leveldb::Slice startkey(JOB_RALATED + jobid + _id_to_string(0));

  //scan all node list related with jobid;
  auto node_list = _scan_range(startkey);

  leveldb::WriteBatch batch;
  //# 第一步 删除有job关联 kv对
  for(const auto& nid : node_list) {
    batch.Delete(RESERVE_RALATED + nid + jobid);
    batch.Delete(REGULAR_RALATED + nid + jobid);
    batch.Delete(JOB_RALATED + jobid + nid);
  }
  batch.Delete(JOBKEY + jobid);

  auto s = _db->Write(leveldb::WriteOptions(), &batch);
  return s.ok();
}

bool LocalCachedUtil::get_fresh_task_list(const uint64_t &node_id,
                                          TaskMap *tasks) {
  const std::string nodeid = _id_to_string(node_id);
  leveldb::Slice startkey(RESERVE_RALATED + nodeid + _id_to_string(0));
  auto jobid_list = _scan_range(startkey);
  TaskDef t;
  for (const auto& id : jobid_list) {
    Slice key(JOBKEY + id);
    std::string val;
    _db->Get(leveldb::ReadOptions(),key, &val);
    if(t.ParseFromString(val))
      tasks->insert({decode_fixed64(id.c_str()),t});
  }
  return true;
}

bool LocalCachedUtil::get_whole_task_list(const uint64_t &node_id,
                                          TaskMap *tasks) {

  const std::string nodeid = _id_to_string(node_id);
  leveldb::Slice startkey(RESERVE_RALATED + nodeid + _id_to_string(0));
  auto jobid_list = _scan_range(startkey);
  TaskDef t;
  for (const auto& id : jobid_list) {
    Slice key(JOBKEY + id);
    std::string val;
    _db->Get(leveldb::ReadOptions(),key, &val);
    if(t.ParseFromString(val))
      tasks->insert({decode_fixed64(id.c_str()),t});
  }

  leveldb::Slice startkey2(REGULAR_RALATED + nodeid + _id_to_string(0));
  auto jobid2_list = _scan_range(startkey);
  for (const auto& id : jobid2_list) {
    Slice key(JOBKEY + id);
    std::string val;
    _db->Get(leveldb::ReadOptions(),key, &val);
    if(t.ParseFromString(val))
      tasks->insert({decode_fixed64(id.c_str()),t});
  }

  return true;
}

bool LocalCachedUtil::store_task_node_ship(const uint64_t &job_id,
                                           const uint64_t &node_id) {
  return false;
}

bool LocalCachedUtil::get_count(const uint64_t &node_id, uint64_t *count) {
  return false;
}

LocalCachedUtil::LocalCachedUtil(const std::string &path, Options *option) {

  _options.create_if_missing = true;

  leveldb::Status status = leveldb::DB::Open(_options, "/tmp/testdb", &_db);
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

std::vector<string> LocalCachedUtil::_scan_range(const Slice &start) {
  std::vector<string> task_list{};
  shared_ptr<leveldb::Iterator> iter(_db->NewIterator(leveldb::ReadOptions()));

  for (iter->Seek(start);
       iter->Valid() && in_range_compare(start, iter->key());
       iter->Next()) {
    task_list.push_back(iter->value().ToString());
  }

  return task_list;
}

//
//void LocalCachedUtil::_scan_range(const Slice &startkey,
//                                  const const Slice &endkey) {
//
//
//}


std::unique_ptr<LocalCachedUtilInterface> LevelDBCachedUtilUniquePtr(
    const std::string &path, Options *option) {

  return make_unique<LocalCachedUtil>(path, option);

}

} //namespace taskserver

} //namspace webmonitor
