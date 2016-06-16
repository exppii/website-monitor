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

  explicit PingTask(const TaskDef *, std::shared_ptr<DataProcServiceInterface> dataproc);

  bool reach_time(const std::time_t&) const override;

  bool varify_task_content() const override;

protected:

  bool _do_run() override;

private:


  TaskDef _task_def;

  PING_CONTENT _content;

  std::shared_ptr<DataProcServiceInterface> _dataproc;

};

PingTask::PingTask(const TaskDef *task, std::shared_ptr<DataProcServiceInterface> dataproc)
    : _task_def(*task), _dataproc(dataproc) {
  if (task->content().Is<PING_CONTENT>()) task->content().UnpackTo(&_content);
}

//TODO
bool PingTask::_do_run() {
  const std::string data = "run ping task.";
  _dataproc->add_data(data);
  return false;
}

//TODO
bool PingTask::varify_task_content() const {
  return false;
}


bool PingTask::reach_time(const std::time_t &now) const {
  if(_is_running() || _task_def.status() == TaskDef::STOP) return false;
  auto dif = std::difftime(now, _last_run_time);
  return dif > _task_def.frequency();
}


TaskInterface* NewPingTaskPtr(const TaskDef *task, std::shared_ptr<DataProcServiceInterface> dataproc) {
  return new PingTask(task, dataproc);
}

} //namespace node

} //namespace webmonitor
