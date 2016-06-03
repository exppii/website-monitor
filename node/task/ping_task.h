//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR_NODE_PING_TASK_H_
#define WEBSITEMONITOR_NODE_PING_TASK_H_

#include <memory>

namespace webmonitor {

class TaskDef;
class TaskInterface;
std::shared_ptr<TaskInterface> PingTaskSharedPtr(const TaskDef*);

}

#endif //WEBSITEMONITOR_NODE_PING_TASK_H_
