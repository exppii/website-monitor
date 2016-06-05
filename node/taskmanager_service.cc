//
// Created by 刘优 on 16/6/4.
//

#include "node/taskmanager_service.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

namespace webmonitor {

namespace node {

class NodeTaskManager : public TaskManagerInterface {
public:

  explicit NodeTaskManager(const DataProcServiceInterface* proc):_data_proc(proc){}

  bool add_task() override;
  void start() override ;
  void stop() override ;

private:

  const DataProcServiceInterface* _data_proc;

};

bool NodeTaskManager::add_task() {
  return false;
}

void NodeTaskManager::start() {

}

void NodeTaskManager::stop() {

}


std::unique_ptr<TaskManagerInterface> TaskManagerUniquePtr(const DataProcServiceInterface* proc) {
  return make_unique<NodeTaskManager>(proc);
}

}

}