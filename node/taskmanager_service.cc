//
// Created by 刘优 on 16/6/4.
//

#include "node/taskmanager_service.h"

#include <vector>
#include <thread>
#include <mutex>

#include "node/task/task_interface.h"

#include "node/logger.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

using std::vector;
using std::string;
using std::shared_ptr;
using std::thread;
using std::mutex;

namespace webmonitor {

namespace node {

class NodeTaskManager : public TaskManagerInterface {
public:

  explicit NodeTaskManager(const DataProcServiceInterface* proc):_data_proc(proc){}

  bool add_task(const TaskMapSharedPtr& ) const override;
  void start() override ;
  void stop() override ;

private:

  void work_thread();

private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};

  bool _running{true};

  std::vector<thread> _threads;
  mutex _mtx;

  const DataProcServiceInterface* _data_proc;
  std::unique_ptr<std::map<int64_t,TaskSharedPtr>> _task_map{make_unique<std::map<int64_t,TaskSharedPtr>>()};


};

bool NodeTaskManager::add_task(const TaskMapSharedPtr&  task_map) const {

  // //remove same task in old map
  // for(auto it = _task_map->begin();it != _task_map->end();) {
  //   task_map.find(it->first) != task_map.end() ? it = _task_map->erase(it) : ++it;
  // }
  // //add new tasks to old maps
  // _task_map->insert(task_map.cbegin(),task_map.cend());
  return false;
}

void NodeTaskManager::start() {

}

void NodeTaskManager::stop() {

}


std::unique_ptr<TaskManagerInterface> TaskManagerUniquePtr(const DataProcServiceInterface* proc) {
  return make_unique<NodeTaskManager>(proc);
}

} //namespace node

} //namespace webmonitor
