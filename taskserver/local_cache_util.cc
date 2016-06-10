//
// Created by 刘优 on 16/6/8.
//

#include "taskserver/local_cache_util.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

namespace webmonitor {

namespace taskserver {


class LocalCachedUtil : public LocalCachedUtilInterface {

public:

  using TaskMap = ::google::protobuf::Map< ::google::protobuf::int64, TaskDef>;

  LocalCachedUtil(const std::string& path, Options * option);
  ~LocalCachedUtil();

  bool store_job(const JobDef& job) override;

  bool update_job(const JobDef& job) override;

  bool del_job(const int64_t& job_id) override;

  bool get_fresh_task_list(const int64_t& node_id, TaskMap* tasks) override;

  bool get_whole_task_list(const int64_t& node_id, TaskMap* tasks) override;

  bool store_task_node_ship(const int64_t& job_id, const int64_t& node_id) override;

  bool get_count(const int64_t& node_id, int64_t* count) override;

private:
  //std::unique_ptr<DBClient> _client;
};


//# -----------------batch-------------------
//# 第一步 创建 job kv对 插入数据库
//key = "job" + id
//value = task.dump() #序列化
//# 第二步 创建 node 和 job reserve关联 kv 对
//    key = "reserve" + node_id + "job" + job_id
//# -----------------batch-------------------
bool LocalCachedUtil::store_job(const JobDef& job) {

  const std::string JKEY = "job" + std::to_string(job.id());



  std::string taskstring{};
  job.task().SerializeToString(&taskstring);

  auto l = job.node_list();
  return false;
}

bool LocalCachedUtil::update_job(const JobDef& job) {
  return false;
}

bool LocalCachedUtil::del_job(const int64_t &job_id) {
  return false;
}

bool LocalCachedUtil::get_fresh_task_list(const int64_t &node_id,
                                          TaskMap *tasks) {
  return false;
}

bool LocalCachedUtil::get_whole_task_list(const int64_t &node_id,
                                          TaskMap *tasks) {
  return false;
}

bool LocalCachedUtil::store_task_node_ship(const int64_t &job_id,
                                           const int64_t &node_id) {
  return false;
}

bool LocalCachedUtil::get_count(const int64_t &node_id, int64_t *count) {
  return false;
}

LocalCachedUtil::LocalCachedUtil(const std::string& path, Options * option) {

}

LocalCachedUtil::~LocalCachedUtil() {

}


std::unique_ptr <LocalCachedUtilInterface> LevelDBCachedUtilUniquePtr(
    const std::string& path, Options * option) {

  return make_unique<LocalCachedUtil>(path, option);

}

} //namespace taskserver

} //namspace webmonitor


