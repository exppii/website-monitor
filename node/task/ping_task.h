//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR_NODE_PING_TASK_H_
#define WEBSITEMONITOR_NODE_PING_TASK_H_

#include <memory>

namespace webmonitor {

class TaskDef;

namespace node {

class TaskInterface;

class DataProcServiceInterface;

TaskInterface* NewPingTaskPtr(const TaskDef *, std::shared_ptr<DataProcServiceInterface>);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_PING_TASK_H_
