//
// Created by 刘优 on 16/6/2.
//


#include "node/task/dns_task.h"

#include "node/task/task_interface.h"

#include "node/data_proc_service.h"

#include "protos/master_service.pb.h"
#include "protos/task_content.pb.h"

using std::string;

namespace webmonitor {

namespace node {
class DNSTask : public TaskInterface {
public:

  explicit DNSTask(const TaskDef *);

  bool reach_time(const std::time_t &) const override;

  bool varify_task_content() const override;

protected:

  bool _do_run(Closure done) override;

private:

  TaskDef _task_def;

  DNS_CONTENT _content;

};

DNSTask::DNSTask(const TaskDef *task) : _task_def(*task) {
  if (task->content().Is<DNS_CONTENT>()) task->content().UnpackTo(&_content);
}

//TODO
bool DNSTask::_do_run(Closure done) {
  const nlohmann::json n{};
  return done(n);
}

//TODO
bool DNSTask::varify_task_content() const {
  return false;
}


bool DNSTask::reach_time(const std::time_t &now) const {
  if (_is_running() || _task_def.status() == TaskDef::STOP) return false;
  auto dif = std::difftime(now, _last_run_time);
  return dif > _task_def.frequency();
}


TaskInterface *NewDNSTaskPtr(const TaskDef *task) {
  return new DNSTask(task);
}

} //namespace node

} //namespace
