//
// Created by 刘优 on 16/6/4.
//

#include "node/taskmanager_service.h"

#include <vector>
#include <thread>
#include <mutex>
#include <future>

#include "node/logger.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

using std::vector;
using std::map;
using std::string;
using std::shared_ptr;
using std::thread;
using std::mutex;
using std::make_shared;

namespace webmonitor {

namespace node {

class NodeTaskManager : public TaskManagerInterface {
public:

  explicit NodeTaskManager() {
    _logger->info("Init task manager service...");
  }

  bool add_task(const std::map<uint64_t, TaskSharedPtr> &) override;

  bool del_task(const std::vector<uint64_t>&) override;

  size_t running_count() override ;

  void start() override;

  void stop() override;

private:

  void _work_thread();

  void _mv_reserved_to_regular();

  void _do_assign_task(std::vector<std::future<bool>>*);

private:
  using TaskMapUniquePtr = std::unique_ptr<std::map<uint64_t, TaskSharedPtr>>;

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};

  bool _running{true};

  std::unique_ptr<thread> _thread{nullptr};
  std::mutex _reserve_mtx;
  std::mutex _regular_mtx;
  std::mutex _del_mtx;

  TaskMapUniquePtr _reserve_task{make_unique<map<uint64_t, TaskSharedPtr>>()};

  TaskMapUniquePtr _regular_task{make_unique<map<uint64_t, TaskSharedPtr>>()};

  std::vector<uint64_t> _to_be_del_list;

};

bool NodeTaskManager::add_task(const std::map<uint64_t, TaskSharedPtr> &tasks) {
  std::unique_lock<mutex> mlock(_reserve_mtx);
  _logger->debug("remove same task in reserve task list.");
  for (auto t = _reserve_task->begin(); t != _reserve_task->end();) {
    tasks.find(t->first) != tasks.end() ? t = _reserve_task->erase(t) : ++t;
  }

  _reserve_task->insert(tasks.cbegin(), tasks.cend());
  _logger->info("add tasks to reserve task list, current reserve task size {}", _reserve_task->size());
  return false;
}

bool NodeTaskManager::del_task(const std::vector<uint64_t>& ids) {
  std::unique_lock<mutex> mlock(_del_mtx);
  _to_be_del_list.insert(_to_be_del_list.end(),ids.cbegin(), ids.cend());
  _logger->debug("add drop list to 'to be del list', size: {}. ", ids.size());
  return true;
}

void NodeTaskManager::start() {
  _logger->info("start task manager thread...");

  _running = true;

  _thread.reset(new thread(&NodeTaskManager::_work_thread, this));

}

void NodeTaskManager::stop() {
  _running = false;

  if (_thread && _thread->joinable()) _thread->join();
  _logger->info("task manager thread stopped.");
}

void NodeTaskManager::_work_thread() {

  _logger->info("init async result vector for task.");
  std::vector<std::future<bool> > task_result;

  while (_running) {

    {
      std::unique_lock<mutex> mlock(_regular_mtx);
      //drop expired task
      {
        std::unique_lock<mutex> mlock(_del_mtx);
        if(_to_be_del_list.size() > 0) {
          for(const auto& j : _to_be_del_list) {
            _logger->info("job {} is delete by server, now remove it from regular list,", j);
            _regular_task->erase(j);
          }
          _to_be_del_list.clear();
        }

      }

      _mv_reserved_to_regular();

      _logger->debug("current regular list size: {}.", _regular_task->size());
    }


    _do_assign_task(&task_result);

    std::this_thread::sleep_for(std::chrono::seconds(3));
  }

}

//mv reserve_task
void NodeTaskManager::_mv_reserved_to_regular() {
  std::unique_lock<mutex> mlock(_reserve_mtx);
  //remove task if id is contained in reserved task
  if(_reserve_task->size() > 0) {
    _logger->debug("New task size is {}. Add to run list.",_reserve_task->size());

    for (auto t = _regular_task->begin(); t != _regular_task->end();) {
      _reserve_task->find(t->first) != _reserve_task->end()
      ? t = _regular_task->erase(t) : ++t;
    }
    //mv new tasks to old maps
    _regular_task->insert(_reserve_task->cbegin(), _reserve_task->cend());
    _reserve_task->clear();
  }


}

void NodeTaskManager::_do_assign_task(std::vector<std::future<bool>>* result) {
  std::time_t now = std::time(nullptr);
  for(auto& task : *_regular_task) {
    if(task.second->reach_time(now)) {
      result->push_back(std::async(std::launch::async,&TaskInterface::run, task.second.get()));
    }
  }

  result->erase(std::remove_if(result->begin(), result->end(), [](std::future<bool> &f) {
    return (f.wait_for(std::chrono::seconds(0)) == std::future_status::ready);
  }), result->end());

}

size_t NodeTaskManager::running_count() {
  std::unique_lock<mutex> mlock(_regular_mtx);
  return _regular_task->size();
}


std::unique_ptr<TaskManagerInterface> TaskManagerUniquePtr() {
  return make_unique<NodeTaskManager>();
}

} //namespace node

} //namespace webmonitor
