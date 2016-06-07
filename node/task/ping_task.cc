//
// Created by 刘优 on 16/6/2.
//

#include "node/task/ping_task.h"
#include "node/task/task_interface.h"
#include "protos/master_service.pb.h"
#include "protos/task_content.pb.h"

#include "node/data_proc_service.h"

#include "common/json.hpp"

using std::string;
namespace webmonitor {


namespace node {
class PingTask : public TaskInterface {
public:

  explicit PingTask(const TaskDef *, DataProcServiceInterface *dataproc);

  bool run() override;

  bool reach_time(const std::time_t &) override;

  bool varify_task_content() const override;

  bool is_expired() const override;

private:


  TaskDef _task_def;

  PING_CONTENT _content;

  DataProcServiceInterface *_dataproc;

};

PingTask::PingTask(const TaskDef *task, DataProcServiceInterface *dataproc)
    : _task_def(*task), _dataproc(dataproc) {
  if (task->content().Is<PING_CONTENT>()) task->content().UnpackTo(&_content);
}

//TODO
bool PingTask::run() {
  const std::string data = "run ping task.";
  _dataproc->add_data(data);
  return false;
}

//TODO
bool PingTask::varify_task_content() const {
  return false;
}

bool PingTask::is_expired() const {
  return _task_def.status() == TaskDef::EXPIRE;
}

bool PingTask::reach_time(const std::time_t &now) {
  auto dif = std::difftime(now, _last_run_time);
  return dif > _task_def.frequency();
}


std::shared_ptr<TaskInterface>
PingTaskSharedPtr(const TaskDef *task, DataProcServiceInterface *dataproc) {
  return std::make_shared<PingTask>(task, dataproc);
}

} //namespace node

} //namespace webmonitor
