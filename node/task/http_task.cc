//
// Created by 刘优 on 16/6/2.
//

#include "node/task/http_task.h"

#include <string>

#include "node/task/task_interface.h"
#include "protos/task_content.pb.h"

#include "node/data_proc_service.h"
#include "node/response/curl_response.h"
#include "node/curl/curl_lib.h"
#include "protos/master_service.pb.h"


using std::string;

namespace webmonitor {

namespace node {
class HttpTask :public TaskInterface {
public:

  explicit HttpTask(const TaskDef *, DataProcServiceInterface *dataproc);

  bool reach_time(const std::time_t&) const override;

  bool varify_task_content() const override;

protected:

  bool _do_run() override;

private:


  TaskDef _task_def;

  HTTP_CONTENT _content;

  DataProcServiceInterface *_dataproc;

};

HttpTask::HttpTask(const TaskDef *task, DataProcServiceInterface *dataproc)
    : _task_def(*task), _dataproc(dataproc) {
  if(task->content().Is<HTTP_CONTENT>()) task->content().UnpackTo(&_content);
}

//TODO
bool HttpTask::_do_run() {

  auto ret = false;
  node::CurlResponse resp;
  
  switch (_content.method()) {
      
    case HTTP_CONTENT::GET:
      curl_get(_task_def.dest(),&resp);
      break;
    case HTTP_CONTENT::HEAD:
      curl_head(_task_def.dest(),&resp);
      break;
    case HTTP_CONTENT::POST:
      curl_post(_task_def.dest(),&resp);
      break;
    default:
      return ret;
  }
  printf("run http task, match content: %s", _content.match_content().c_str());

  //if _dataproc
  return _dataproc->add_data(resp.to_json());

}

//TODO parser http is valid
bool HttpTask::varify_task_content() const {
  return _content.method() != HTTP_CONTENT::UNKNOWN;
}



bool HttpTask::reach_time(const std::time_t & now) const {
  if(_is_running() || _task_def.status() == TaskDef::STOP) return false;
  auto dif = std::difftime(now, _last_run_time);
  return dif > _task_def.frequency();
}


std::shared_ptr<TaskInterface>
HttpTaskSharedPtr(const TaskDef *task, DataProcServiceInterface *dataproc) {
  return std::make_shared<HttpTask>(task, dataproc);
}

} //namespace node

} //namespace webmonitor
