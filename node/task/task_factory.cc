//
// Created by 刘优 on 16/6/2.
//

#include "node/task/task_factory.h"

#include "node/task/http_task.h"
#include "node/task/dns_task.h"
#include "node/task/ping_task.h"

#include "common/json.hpp"
#include "protos/master_service.pb.h"

using std::make_shared;
namespace webmonitor {

std::shared_ptr<TaskInterface> TaskFactory::create(const TaskDef* task) {
  std::shared_ptr<TaskInterface> interface = nullptr;

  switch (task->proto()) {
    case TaskDef::HTTP: {
      interface = HttpTaskSharedPtr(task);
      break;
    }
    case TaskDef::PING: {
      interface = PingTaskSharedPtr(task);
      break;
    }
    case TaskDef::DNS: {
      interface = PingTaskSharedPtr(task);
      break;
    }
    case TaskDef::UNKNOWN:
    default:
      break;
  }

  if(interface && interface->varify_task_content()) {
    return interface;
  } else {
    return nullptr;
  }

}

} // namespace webmonitor
