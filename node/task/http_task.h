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

class DataProcServiceInterface;

TaskInterface* NewHttpTaskPtr(const TaskDef *, std::shared_ptr<DataProcServiceInterface>);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_HTTP_TASK_H
