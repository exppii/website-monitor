//
// Created by 刘优 on 16/5/13.
//
#include <grpc++/support/status.h>
#include "taskserver/master.h"

namespace webmonitor {

void Master::create_job(const CreateJobRequest *req, CreateJobResponse *resp,
                              Master::closure done) {
  //TODO
  //_local_cache.add(req->jobs(0));

  //_reply.status().set_response_code(ResponseStatusDef_STATUS_SUCCESS);
  // ResponseStatusDef res;

  auto res = resp->mutable_response();
  res->set_message("success add task to queue.");
  res->set_status(RespStatusDef_STATUS_SUCCESS);
  done(grpc::Status::OK);
}

void Master::update_job(const UpdateJobRequest *req, UpdateJobResponse *resp,
                              Master::closure done) {
  //TODO
  //auto ret = _local_cache.update(req->jobs(0));
  if(1) {
    auto res = resp->mutable_response();
    res->set_message("success update task to queue.");
    res->set_status(RespStatusDef_STATUS_SUCCESS);
  } else {
    auto res = resp->mutable_response();
    res->set_message("fail update task to queue.");
    res->set_status(RespStatusDef_STATUS_FAILED);
  }
  done(grpc::Status::OK);
}

void Master::delete_job(const DeleteJobRequest *req, DeleteJobResponse *resp,
                        closure done) {
  //TODO
  //auto ret = _local_cache.update(req->jobs(0));
  if(1) {
    auto res = resp->mutable_response();
    res->set_message("success update task to queue.");
    res->set_status(RespStatusDef_STATUS_SUCCESS);
  } else {
    auto res = resp->mutable_response();
    res->set_message("fail update task to queue.");
    res->set_status(RespStatusDef_STATUS_FAILED);
  }
  done(grpc::Status::OK);
}

void Master::list_job_status(const ListJobStatusRequest *req,
                             ListJobStatusResponse *resp, closure done) {
  //TODO
  done(grpc::Status::OK);
}

void Master::list_node_status(const ListNodeStatusRequest *req,
                              ListNodeStatusResponse *resp, closure done) {
  //TODO
  done(grpc::Status::OK);
}

void Master::report_status(const ReportStatusRequest *req,
                           ReportStatusResponse *resp, closure done) {
  //TODO
  done(grpc::Status::OK);
}



void Master::get_job(const GetJobRequest *req, GetJobResponse *resp,
                     closure done) {
  //TODO
  std::cout << req->node().id() << std::endl;
  for (int i = 0; i < 10; ++i) {
    auto t = resp->add_task_list();
    t->set_id(i*100);
    t->set_content("task content");

  }

  done(grpc::Status::OK);
}


} //namespace elon





