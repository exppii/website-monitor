//
// Created by 刘优 on 16/6/2.
//


#include "node/task/dns_task.h"

#include "node/task/task_interface.h"
#include "protos/master_service.pb.h"
#include "protos/task_content.pb.h"

using std::string;

namespace webmonitor {

namespace node {
class DNSTask :public TaskInterface {
public:

  explicit DNSTask(const TaskDef*);

  void run() override;

  bool varify_task_content() const override;

  bool is_expired() const override;
private:

  TaskDef _task_def;

  DNS_CONTENT _content;

};

DNSTask::DNSTask(const TaskDef* task):_task_def(*task) {
  if(task->content().Is<DNS_CONTENT>()) task->content().UnpackTo(&_content);
}

void DNSTask::run() {
  printf("run dns task, taskid: %lld", 100L);
}

//TODO
bool DNSTask::varify_task_content() const {
  return false;
}

bool DNSTask::is_expired() const {
  return _task_def.status() == TaskDef::EXPIRE;
}


std::shared_ptr<TaskInterface> DNSTaskSharedPtr(const TaskDef* task) {
  return std::make_shared<DNSTask>(task);


}

} //namespace node

} //namespace
