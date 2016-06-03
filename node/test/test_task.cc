//
// Created by 刘优 on 16/6/2.
//

#include <iostream>

#include "node/task/task_factory.h"
#include "protos/master_service.pb.h"

using namespace std;
using namespace webmonitor;

JobDef create_job(const int id, const std::string& content){
  JobDef job;

  auto task = job.mutable_task();

  task->set_id(id);
  task->set_content(content);
  task->set_proto(TaskDef::HTTP);

  job.set_status(JobDef::RUNNING);

  job.add_worker_ids(1);
  job.add_worker_ids(2);

  return job;
}


int main(int argc, char const *argv[]) {

  const std::string http_task = R"({
  "http_method" : 1,
  "resp_context" : "this is correct data",
  "match_method" : 3
})";


  TaskFactory factory;

  auto job = create_job(1000,http_task);

  auto task = job.task();

  auto tk = factory.create(&task);

  if(tk)
    tk->run();
  else
    cout << "tk is nullptr" << endl;




}