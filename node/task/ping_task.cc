//
// Created by 刘优 on 16/6/2.
//

#include "node/task/ping_task.h"
#include "node/task/task_interface.h"
#include "protos/master_service.pb.h"
#include "protos/task_content.pb.h"

#include "common/json.hpp"
using std::string;
namespace webmonitor {


namespace node {
class PingTask :public TaskInterface {
public:

  explicit PingTask(const TaskDef*);

  void run() override;

  bool varify_task_content() const override;

private:


  TaskDef _task_def;

  PING_CONTENT _content;

};

PingTask::PingTask(const TaskDef *task):_task_def(*task) {
  if(task->content().Is<PING_CONTENT>()) task->content().UnpackTo(&_content);
}


void PingTask::run() {
  printf("run ping task, taskid: %lld", 100);
}

//TODO
bool PingTask::varify_task_content() const {
  return false;
}

std::shared_ptr<TaskInterface> PingTaskSharedPtr(const TaskDef* task) {
  return std::make_shared<PingTask>(task);
}

} //namespace node

} //namespace webmonitor
