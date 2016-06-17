//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR_NODE_PING_TASK_H_
#define WEBSITEMONITOR_NODE_PING_TASK_H_

namespace webmonitor {

class TaskDef;

namespace node {

class TaskInterface;

TaskInterface* NewPingTaskPtr(const TaskDef *);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_PING_TASK_H_
