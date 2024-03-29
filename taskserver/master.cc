//
// Created by 刘优 on 16/5/13.
//
#include <grpc++/support/status.h>
#include "taskserver/master.h"

namespace webmonitor {

namespace taskserver {

void Master::create_job(const CreateJobRequest *req, CreateJobResponse *resp,
                        Master::closure done) {
  _logger->info("received one create job command.");
  auto res = resp->mutable_response();
  if (_cache_util->store_job(req->job())) {
    res->set_message("success save task to local.");
    res->set_status(RespStatusDef::SUCCESS);
  } else {
    res->set_message("failed to save task to local.");
    res->set_status(RespStatusDef::FAILED);
  }
  done(grpc::Status::OK);
}

void Master::update_job(const UpdateJobRequest *req, UpdateJobResponse *resp,
                        Master::closure done) {
  _logger->info("received one update job command.");
  auto res = resp->mutable_response();
  if (_cache_util->update_job(req->job())) {
    res->set_message("success update task to local.");
    res->set_status(RespStatusDef::SUCCESS);
  } else {
    res->set_message("failed to update task to local.");
    res->set_status(RespStatusDef::FAILED);
  }
  done(grpc::Status::OK);
}

void Master::delete_job(const DeleteJobRequest *req, DeleteJobResponse *resp,
                        closure done) {
  _logger->info("received one delete job command.");
  auto res = resp->mutable_response();
  if (_cache_util->del_job(req->job_id())) {
    res->set_message("success del task from local.");
    res->set_status(RespStatusDef::SUCCESS);
  } else {
    res->set_message("failed to del task from local.");
    res->set_status(RespStatusDef::FAILED);
  }
  done(grpc::Status::OK);
}

void Master::list_job_status(const ListJobStatusRequest *req,
                             ListJobStatusResponse *resp, closure done) {
  _logger->info("received one list job command.");
  //TODO
  done(grpc::Status::OK);
}

void Master::list_node_status(const ListNodeStatusRequest *req,
                              ListNodeStatusResponse *resp, closure done) {
  _logger->info("received one list node command.");
  //TODO
  done(grpc::Status::OK);
}


void Master::get_job(const GetJobRequest *req, GetJobResponse *resp,
                     closure done) {
  _logger->info("received one get job command from node: {}", req->node_id());

  auto l = resp->mutable_dropped_list();
  _cache_util->get_delete_task_list(req->node_id(),l);

  auto m = resp->mutable_task_map();

  auto local = _cache_util->get_count(req->node_id());

  if ((local + l->size()) == req->running_task_count()) {

    _cache_util->get_fresh_task_list(req->node_id(), m);
  } else {
    _logger->warn(
        "running count {} did not match local count {}. current node: {}",
        req->running_task_count(), local, req->node_id());
    _cache_util->get_whole_task_list(req->node_id(), m);
  }

  done(grpc::Status::OK);
}

void Master::report_status(const ReportStatusRequest *req,
                           ReportStatusResponse *resp, closure done) {
  _logger->info("received one get report command from node: {}",
                req->node_id());
  //TODO
  done(grpc::Status::OK);
}


} //namespace taskserver

} //namespace webmonitor
