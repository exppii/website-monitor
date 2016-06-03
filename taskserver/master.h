//
// Created by 刘优 on 16/5/12.
//

#ifndef WEBSITEMONITOR_TASKSERVER_MASTER_H_
#define WEBSITEMONITOR_TASKSERVER_MASTER_H_

#include "task_cache.h"
#include "protos/master_service.pb.h"
#include "protos/master_service.grpc.pb.h"

namespace webmonitor {

class Master {

public:
  Master(){}
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

  TaskCache<JobDef> _local_cache;

};

} // namespace webmonitor

#endif //WEBSITEMONITOR_TASKSERVER_MASTER_H_
