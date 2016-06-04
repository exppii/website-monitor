//
// Created by 刘优 on 16/6/4.
//

#ifndef WEBSITEMONITOR_NODE_TASKMANAGER_SERVICE_H_
#define WEBSITEMONITOR_NODE_TASKMANAGER_SERVICE_H_


#include <memory>

#include "common/service_interface.h"

namespace webmonitor {

namespace node {

class DataProcServiceInterface;

class TaskManagerInterface : public ServiceInterface {

public:
  virtual ~TaskManagerInterface(){}

  virtual bool add_task() = 0;

};

std::unique_ptr<TaskManagerInterface> TaskManagerUniquePtr(const DataProcServiceInterface*);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_TASKMANAGER_SERVICE_H_
