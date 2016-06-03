//
// Created by 刘优 on 16/6/2.
//

#include "node/task/dns_task.h"

#include "node/task/task_interface.h"
#include "protos/master_service.pb.h"
#include "common/json.hpp"

using std::string;

namespace webmonitor {

class DNSTask :public TaskInterface {
public:

  explicit DNSTask(const TaskDef*);

  void run() override;
  std::string dump() const override;
private:

  void _parser_task_content(const TaskDef* task);

  std::string _dns;

};

DNSTask::DNSTask(const TaskDef* task)
  :TaskInterface(task->id(),task->frequency(),task->dest()) {
  _parser_task_content(task);
}

void DNSTask::run() {
  printf("run dns task, taskid: %lld", _ID);
}

void DNSTask::_parser_task_content(const TaskDef *task) {
  try {

    const auto j = nlohmann::json::parse(task->content());

    _dns = j["dns"].get<string>();
    _task_content_ok = true;
  }catch (const std::exception& ex) {
    //log error
  }
}

std::string DNSTask::dump() const {
  return _dns;
}


std::shared_ptr<TaskInterface> DNSTaskSharedPtr(const TaskDef* task) {
  return std::make_shared<DNSTask>(task);


}

} //namspace
