//
// Created by 刘优 on 16/5/12.
//

#ifndef WEBSITEMONITOR_TASKSERVER_MASTER_H_
#define WEBSITEMONITOR_TASKSERVER_MASTER_H_


#include "taskserver/local_cache_util.h"
#include "taskserver/logger.h"
#include "protos/master_service.pb.h"
#include "protos/master_service.grpc.pb.h"


namespace webmonitor {

namespace taskserver {

class Master {

public:
  Master(Options * options)
      :_cache_util(LevelDBCachedUtilUniquePtr(options)) {}
// Convenient typedef for a closure passing a Status.
  typedef std::function<void(const ::grpc::Status&)> closure;

  //for web client
  void create_job(const CreateJobRequest* req, CreateJobResponse* resp, closure done);
  void update_job(const UpdateJobRequest* req, UpdateJobResponse* resp, closure done);
  void delete_job(const DeleteJobRequest* req, DeleteJobResponse* resp, closure done);
  void list_job_status(const ListJobStatusRequest* req, ListJobStatusResponse* resp, closure done);
  void list_node_status(const ListNodeStatusRequest* req, ListNodeStatusResponse* resp, closure done);

  //for node client
  void report_status(const ReportStatusRequest* req, ReportStatusResponse* resp, closure done);
  void get_job(const GetJobRequest* req, GetJobResponse* resp, closure done);

private:

  bool _running_task_count_match_in_local(const uint64_t& node_id, const uint64_t& count);

private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(SERVER_TAG)};
  std::unique_ptr <LocalCachedUtilInterface> _cache_util;

};

} //namespace taskserver

} // namespace webmonitor

#endif //WEBSITEMONITOR_TASKSERVER_MASTER_H_
