//
// Created by 刘优 on 16/6/2.
//

#include "node/task/http_task.h"

#include <string>

#include "node/task/task_interface.h"
#include "protos/task_content.pb.h"
#include "protos/master_service.pb.h"


using std::string;

namespace webmonitor {

class HttpTask :public TaskInterface {
public:


  explicit HttpTask(const TaskDef*);

  void run() override;

  bool varify_task_content() const override;

private:

  TaskDef _task_def;

  HTTP_CONTENT _content;

};


HttpTask::HttpTask(const TaskDef* task):_task_def(*task) {
  if(task->content().Is<HTTP_CONTENT>()) task->content().UnpackTo(&_content);
}

void HttpTask::run() {
  printf("run http task, match content: %s", _content.match_content().c_str());
}

//TODO parser http is valid
bool HttpTask::varify_task_content() const {
  return true;
}


std::shared_ptr<TaskInterface> HttpTaskSharedPtr(const TaskDef* task) {
  return std::make_shared<HttpTask>(task);
}

}
