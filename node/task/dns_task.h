//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR_NODE_DNS_TASK_H_
#define WEBSITEMONITOR_NODE_DNS_TASK_H_

#include <memory>

namespace webmonitor {

class TaskDef;
class TaskInterface;

std::shared_ptr<TaskInterface> DNSTaskSharedPtr(const TaskDef*);

}

#endif //WEBSITEMONITOR_NODE_DNS_TASK_H_
