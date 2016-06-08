//
// Created by 刘优 on 16/6/8.
//

#include "taskserver/local_cache_util.h"


namespace webmonitor {

namespace taskserver {

bool LocalCachedUtil::store_job(const JobDef& job) {



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

LocalCachedUtil::LocalCachedUtil() {

}

LocalCachedUtil::~LocalCachedUtil() {

}


} //namespace taskserver

} //namspace webmonitor


