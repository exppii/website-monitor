//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR_TASK_FACTORY_H
#define WEBSITEMONITOR_TASK_FACTORY_H

#include <memory>
#include "node/task/task_interface.h"

namespace webmonitor {

class TaskDef;

namespace node {

class DataProcServiceInterface;

class TaskFactory {
public:
  TaskFactory(){}

  std::shared_ptr<TaskInterface> create(const TaskDef*, DataProcServiceInterface *);

};
}



}

#endif //WEBSITEMONITOR_TASK_FACTORY_H
