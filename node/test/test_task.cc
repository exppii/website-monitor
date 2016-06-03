//
// Created by 刘优 on 16/6/2.
//

#include <iostream>

#include "node/task/task_factory.h"
#include "protos/master_service.pb.h"
#include "protos/task_content.pb.h"

using namespace std;
using namespace webmonitor;

JobDef create_job(){

  JobDef job;

  auto task = job.mutable_task();

  task->set_status(TaskDef::RUN);
  task->set_type(TaskDef::HTTP);

  HTTP_CONTENT content;
  content.set_match_cmd(HTTP_CONTENT::NOT_CARE);
  content.set_method(HTTP_CONTENT::GET);
  content.set_match_content("404");

  task->mutable_content()->PackFrom(content);

  job.add_node_list(1);
  job.add_node_list(2);

  return job;
}


int main(int argc, char const *argv[]) {

  const std::string http_task = R"({
  "http_method" : 1,
  "resp_context" : "this is correct data",
  "match_method" : 3
})";


  TaskFactory factory;

  auto job = create_job();

  auto task = job.task();

  auto tk = factory.create(&task);

  if(tk)
    tk->run();
  else
    cout << "tk is nullptr" << endl;




}