//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR_NODE_DNS_TASK_H_
#define WEBSITEMONITOR_NODE_DNS_TASK_H_

namespace webmonitor {

class TaskDef;

namespace node {

class TaskInterface;

TaskInterface* NewDNSTaskPtr(const TaskDef *);

} //namespace node

} //namspace webmonitor

#endif //WEBSITEMONITOR_NODE_DNS_TASK_H_
