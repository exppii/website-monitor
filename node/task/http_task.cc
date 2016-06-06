//
// Created by 刘优 on 16/6/2.
//

#include "node/task/http_task.h"

#include <string>

#include "node/task/task_interface.h"
#include "protos/task_content.pb.h"
#include "protos/master_service.pb.h"

#include "node/response/curl_response.h"
#include "node/curl/curl_lib.h"

using std::string;

namespace webmonitor {

namespace node {
class HttpTask :public TaskInterface {
public:


  explicit HttpTask(const TaskDef*);

  void run() override;

  bool varify_task_content() const override;

private:

  bool _send_result(const std::string& result);

  TaskDef _task_def;

  HTTP_CONTENT _content;

};


HttpTask::HttpTask(const TaskDef* task):_task_def(*task) {
  if(task->content().Is<HTTP_CONTENT>()) task->content().UnpackTo(&_content);
}

void HttpTask::run() {


  node::CurlResponse resp;

  curl_get(_task_def.dest(),&resp);

  _send_result(resp.dump());

  printf("run http task, match content: %s", _content.match_content().c_str());
}

//TODO parser http is valid
bool HttpTask::varify_task_content() const {
  return true;
}

bool HttpTask::_send_result(const std::string &result) {

  printf(result.c_str());
  return false;
}


std::shared_ptr<TaskInterface> HttpTaskSharedPtr(const TaskDef* task) {
  return std::make_shared<HttpTask>(task);
}

} //namespace node

} //namespace webmonitor
