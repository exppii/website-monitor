//
// Created by 刘优 on 16/6/2.
//

#include "node/task/http_task.h"

#include <string>

#include "node/task/task_interface.h"
#include "protos/master_service.pb.h"

#include "common/json.hpp"

using std::string;

namespace webmonitor {

class HttpTask :public TaskInterface {
public:


  explicit HttpTask(const TaskDef*);

  void run() override;

  virtual std::string dump() const override;

private:

  void _parser_task_content(const TaskDef* task);

//  |请求方法(HTTP_REQ_CMD)|`HEAD`|1|head|
//  |请求方法(HTTP_REQ_CMD)|`GET`|2|get|
//  |请求方法(HTTP_REQ_CMD)|`POST`|3|post|
//  |匹配方法(MATCH_CMD)|`YES`|1|匹配响应|
//  |匹配方法(MATCH_CMD)|`NO`|2|不匹配响应|
//  |匹配方法(MATCH_CMD)|`NOT_CARE`|3|不关心|

  enum METHOD { HEAD = 1, GET, POST };
  enum MATCH {YES =1, NO, NOT_CARE};

  METHOD _method{HEAD};
  MATCH _match_or_not{NOT_CARE};

  std::string _content;

};

//  {
//    "frequency"  : 10,
//    "dest" : "http://website.com",
//    "http_method" : 1,
//    "resp_context" : "this is correct data",
//    "match_method" : 3,
//  }

HttpTask::HttpTask(const TaskDef* task)
    :TaskInterface(task->id(),task->frequency(),task->dest()) {
  _parser_task_content(task);
}

void HttpTask::run() {
  printf("run http task, taskid: %lld", _ID);
}

void HttpTask::_parser_task_content(const TaskDef* task) {
  try {

    const auto j = nlohmann::json::parse(task->content());
    _content = j["resp_context"].get<string>();
    _method = static_cast<METHOD>(j["http_method"].get<int>());
    _match_or_not = static_cast<MATCH>(j["match_method"].get<int>());
    _task_content_ok = true;
  }catch (const std::exception& ex) {
   //log error
  }

}

std::string HttpTask::dump() const {
  std::string ret = _content;
  ret += std::to_string(_method);
  ret += std::to_string(_match_or_not);
  return ret;
}


std::shared_ptr<TaskInterface> HttpTaskSharedPtr(const TaskDef* task) {
  return std::make_shared<HttpTask>(task);
}

}