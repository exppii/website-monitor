//
// Created by 刘优 on 16/6/2.
//

#include "node/task/task_factory.h"

#include "node/data_proc_service.h"

#include "node/task/http_task.h"
#include "node/task/dns_task.h"
#include "node/task/ping_task.h"

#include "protos/task_content.pb.h"
#include "protos/master_service.pb.h"

using std::make_shared;
namespace webmonitor {

namespace node {

std::shared_ptr<TaskInterface>
TaskFactory::create(const TaskDef *task, std::shared_ptr<DataProcServiceInterface> dataproc) {
  std::shared_ptr<TaskInterface> interface = nullptr;

  switch (task->type()) {
    case TaskDef::HTTP: {
      interface.reset(NewHttpTaskPtr(task, dataproc));
      break;
    }
    case TaskDef::PING: {
      interface.reset(NewPingTaskPtr(task, dataproc));
      break;
    }
    case TaskDef::DNS: {
      interface.reset(NewDNSTaskPtr(task, dataproc));
      break;
    }
    case TaskDef::UNKNOWN:
    default:
      break;
  }

  if (interface && interface->varify_task_content()) {
    return interface;
  } else {
    return nullptr;
  }
}

} //namespace node

} //namespace webmonitor
