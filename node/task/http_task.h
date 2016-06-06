//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR_HTTP_TASK_H
#define WEBSITEMONITOR_HTTP_TASK_H

#include <memory>

namespace webmonitor {

class TaskDef;

namespace node {

class TaskInterface;

std::shared_ptr<TaskInterface> HttpTaskSharedPtr(const TaskDef*);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_HTTP_TASK_H
