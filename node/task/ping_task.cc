//
// Created by 刘优 on 16/6/2.
//

#include "node/task/ping_task.h"
#include "node/task/task_interface.h"
#include "protos/master_service.pb.h"

#include "common/json.hpp"
using std::string;
namespace webmonitor {

class PingTask :public TaskInterface {
public:

  explicit PingTask(const TaskDef*);

  void run() override;
  std::string dump() const override ;
private:

  void _parser_task_content(const TaskDef* task);

  int _pkg_num;
  int _pkg_size;


};

PingTask::PingTask(const TaskDef* task)
    :TaskInterface(task->id(),task->frequency(),task->dest()) {
  _parser_task_content(task);
}

void PingTask::run() {
  printf("run ping task, taskid: %lld", _ID);
}

void PingTask::_parser_task_content(const TaskDef *task) {
  try {
    const auto j = nlohmann::json::parse(task->content());
    _pkg_num = j["pkg_num"].get<int>();
    _pkg_size = j["pkg_size"].get<int>();

    _task_content_ok = true;
  }catch (const std::exception& ex) {
    //log error
  }
}

std::string PingTask::dump() const {
  return std::to_string(_pkg_num) + std::to_string(_pkg_size);
}


std::shared_ptr<TaskInterface> PingTaskSharedPtr(const TaskDef* task) {
  return std::make_shared<PingTask>(task);
}

} //namespace
