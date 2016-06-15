//
// Created by 刘优 on 16/6/10.
//

#ifndef WEBSITEMONITOR_TASKSERVER_LOCAL_CACHE_UTIL_INTERFACE_H_
#define WEBSITEMONITOR_TASKSERVER_LOCAL_CACHE_UTIL_INTERFACE_H_


#include <memory>
#include <vector>
#include <cstdint>

#include "protos/master_service.pb.h"

namespace webmonitor {

namespace taskserver {

class LocalCachedUtilInterface {

public:

  using TaskMap = ::google::protobuf::Map< ::google::protobuf::uint64, TaskDef>;
  using IdList = ::google::protobuf::RepeatedField<::google::protobuf::uint64>;

  LocalCachedUtilInterface() = default;
  virtual ~LocalCachedUtilInterface(){}
  //disable copy asign and move constructor
  LocalCachedUtilInterface(const LocalCachedUtilInterface&) = delete;
  LocalCachedUtilInterface& operator=(const LocalCachedUtilInterface&) = delete;
  LocalCachedUtilInterface(LocalCachedUtilInterface&&) = delete;

  virtual bool store_job(const JobDef& job) = 0;

  virtual bool update_job(const JobDef& job) = 0;

  virtual bool del_job(const uint64_t& job_id) = 0;

  virtual bool get_fresh_task_list(const uint64_t& node_id, TaskMap* tasks) = 0;

  virtual bool get_whole_task_list(const uint64_t& node_id, TaskMap* tasks) = 0;

  virtual uint64_t get_count(const uint64_t& node_id) = 0;

  virtual bool get_delete_task_list(const uint64_t& node_id,  IdList* ids) = 0;

  //for test
  virtual uint64_t get_range_count(const std::string& range) = 0;

#ifndef NDEBUG
  //for test
  virtual void print_all() = 0;
#endif
};

} //namespace taskserver

} //namespace webmonitor

#endif //WEBSITEMONITOR_TASKSERVER_LOCAL_CACHE_UTIL_INTERFACE_H_
