//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR_HTTP_TASK_H
#define WEBSITEMONITOR_HTTP_TASK_H

namespace webmonitor {

class TaskDef;

namespace node {

class TaskInterface;

TaskInterface* NewHttpTaskPtr(const TaskDef *);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_HTTP_TASK_H
