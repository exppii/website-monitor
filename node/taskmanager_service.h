//
// Created by 刘优 on 16/6/4.
//

#ifndef WEBSITEMONITOR_NODE_TASKMANAGER_SERVICE_H_
#define WEBSITEMONITOR_NODE_TASKMANAGER_SERVICE_H_


#include <memory>
#include <map>
#include <vector>
#include <node/task/task_interface.h>

#include "common/service_interface.h"

namespace webmonitor {

namespace node {

class TaskInterface;
class DataProcServiceInterface;

using TaskSharedPtr = std::shared_ptr<TaskInterface>;

//using TaskMapSharedPtr = std::shared_ptr< std::map<int64_t, TaskSharedPtr > >;

class TaskManagerInterface : public ServiceInterface {

public:
  virtual ~TaskManagerInterface(){}

  virtual bool add_task(const std::map<uint64_t, TaskSharedPtr>& ) = 0;

  virtual bool del_task(const std::vector<uint64_t>&) = 0;

  virtual size_t running_count() = 0;

};

TaskManagerInterface* NewTaskManagerPtr();

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_TASKMANAGER_SERVICE_H_
