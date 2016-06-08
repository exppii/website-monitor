//
// Created by 刘优 on 16/6/8.
//

#ifndef WEBSITEMONITOR_TASKSERVER_LOCAL_CACHE_UTIL_H_
#define WEBSITEMONITOR_TASKSERVER_LOCAL_CACHE_UTIL_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "protos/master_service.pb.h"

namespace webmonitor {

namespace taskserver {

class DBClient;

class LocalCachedUtil {

public:

  using TaskMap = ::google::protobuf::Map< ::google::protobuf::int64, TaskDef>;

  LocalCachedUtil();
  ~LocalCachedUtil();
  //disable copy asign and move constructor
//  LocalCachedUtil(const LocalCachedUtil&) = delete;
//  LocalCachedUtil& operator=(const LocalCachedUtil&) = delete;
//  LocalCachedUtil(LocalCachedUtil&&) = delete;

  bool store_job(const JobDef& job);

  bool update_job(const JobDef& job);

  bool del_job(const int64_t& job_id);

  bool get_fresh_task_list(const int64_t& node_id, TaskMap* tasks);

  bool get_whole_task_list(const int64_t& node_id, TaskMap* tasks);

  bool store_task_node_ship(const int64_t& job_id, const int64_t& node_id);

  bool get_count(const int64_t& node_id, int64_t* count);

private:
  //std::unique_ptr<DBClient> _client;
};

} //namespace taskserver

} //namspace webmonitor

#endif //WEBSITEMONITOR_TASKSERVER_LOCAL_CACHE_UTIL_H_
