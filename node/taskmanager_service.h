//
// Created by 刘优 on 16/6/4.
//

#ifndef WEBSITEMONITOR_NODE_TASKMANAGER_SERVICE_H_
#define WEBSITEMONITOR_NODE_TASKMANAGER_SERVICE_H_


#include <memory>
#include <map>
#include <node/task/task_interface.h>

#include "common/service_interface.h"

namespace webmonitor {

namespace node {

class TaskInterface;
class DataProcServiceInterface;

using TaskSharedPtr = std::shared_ptr<TaskInterface>;

using TaskMapSharedPtr = std::shared_ptr< std::map<int64_t, TaskSharedPtr > >;

class TaskManagerInterface : public ServiceInterface {

public:
  virtual ~TaskManagerInterface(){}

  virtual bool add_task(const TaskMapSharedPtr& ) const = 0;

};

std::unique_ptr<TaskManagerInterface> TaskManagerUniquePtr(const DataProcServiceInterface*);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_TASKMANAGER_SERVICE_H_
