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
using std::vector;
using std::string;
using std::shared_ptr;
using std::unique_ptr;
using std::thread;
using std::mutex;
using std::make_shared;
using std::chrono::seconds;

namespace webmonitor {

namespace node {

class NodeTaskManager : public TaskManagerInterface {
public:

  explicit NodeTaskManager() {
    _logger->info("Init task manager service...");
  }

  bool add_task(const std::map<uint64_t, TaskSharedPtr> &) override;

  bool del_task(const std::vector<uint64_t> &) override;

  size_t running_count() override;

  void start() override;

  void stop() override;

private:

  void _work_thread();

  void _run_task_thread();

  void _mv_reserved_to_regular();

  void _delete_expired_task();

  void copy_task_running_map();

  void _do_assign_task(map<uint64_t, std::future<bool>> *);

private:
  using TaskMapUniquePtr = std::unique_ptr<std::map<uint64_t, TaskSharedPtr>>;

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};

  bool _running{true};

  std::vector<thread> _threads{};
  std::mutex _reserve_mtx;
  std::mutex _regular_mtx;
  std::mutex _run_mtx;
  std::mutex _del_mtx;

  TaskMapUniquePtr _reserve_task{make_unique<map<uint64_t, TaskSharedPtr>>()};

  TaskMapUniquePtr _regular_task{make_unique<map<uint64_t, TaskSharedPtr>>()};

//copy running task point lest object destoryed when_regular_task erase the job
  TaskMapUniquePtr _running_task{make_unique<map<uint64_t, TaskSharedPtr>>()};

  std::vector<uint64_t> _to_be_del_list;

};

bool NodeTaskManager::add_task(const std::map<uint64_t, TaskSharedPtr> &tasks) {
  std::unique_lock<mutex> mlock(_reserve_mtx);
  _logger->debug("remove same task in reserve task list.");
  for (auto t = _reserve_task->begin(); t != _reserve_task->end();) {
    tasks.find(t->first) != tasks.end() ? t = _reserve_task->erase(t) : ++t;
  }

  _reserve_task->insert(tasks.cbegin(), tasks.cend());
  _logger->info("add tasks to reserve task list, current reserve task size {}",
                _reserve_task->size());
  return false;
}

bool NodeTaskManager::del_task(const std::vector<uint64_t> &ids) {
  std::unique_lock<mutex> mlock(_del_mtx);
  _to_be_del_list.insert(_to_be_del_list.end(), ids.cbegin(), ids.cend());
  _logger->debug("add drop list to 'to be del list', size: {}. ", ids.size());
  return true;
}

void NodeTaskManager::start() {
  _logger->info("start task manager thread...");

  _running = true;

  _threads.push_back(thread(&NodeTaskManager::_work_thread, this));
  _threads.push_back(thread(&NodeTaskManager::_run_task_thread, this));
}

void NodeTaskManager::stop() {
  _running = false;

  for (auto &t : _threads) {
    if (t.joinable()) t.join();
  }

  _logger->info("task manager thread stopped.");
}

void NodeTaskManager::_work_thread() {

  while (_running) {

    {
      std::unique_lock<mutex> mlock(_regular_mtx);

      _delete_expired_task();

      _mv_reserved_to_regular();

      copy_task_running_map();

      _logger->debug("current regular list size: {}.", _regular_task->size());
    }

    std::this_thread::sleep_for(seconds(3));
  }

}

void NodeTaskManager::_run_task_thread() {

  _logger->info("init async result vector for task.");

  auto task_result = make_unique<map<uint64_t, std::future<bool>>>();
  while (_running) {

    {
      std::unique_lock<mutex> mlock(_run_mtx);

      if(_running_task->size() > 0 ) {

        _logger->debug("current running task size {}.", _running_task->size());

        for (auto &task : *_running_task) {
          if(task_result->find(task.first) == task_result->end()) {
            (*task_result)[task.first] = std::async(std::launch::async,
                                                    &TaskInterface::run,
                                                    task.second.get());
          }
        }

        for (auto t = task_result->begin(); t != task_result->end();) {
          if (t->second.wait_for(seconds(0)) == std::future_status::ready) {
            _logger->debug("task {} is finish.", t->first);
            _running_task->erase(t->first);
            t = task_result->erase(t);
          } else {
            ++t;
          }
        }

      }
    }

    std::this_thread::sleep_for(seconds(3));
  }
}


//mv reserve_task
void NodeTaskManager::_mv_reserved_to_regular() {
  std::unique_lock<mutex> mlock(_reserve_mtx);
  //remove task if id is contained in reserved task
  if (_reserve_task->size() > 0) {
    _logger->debug("New task size is {}. Add to run list.",
                   _reserve_task->size());

    for (auto t = _regular_task->begin(); t != _regular_task->end();) {
      _reserve_task->find(t->first) != _reserve_task->end()
      ? t = _regular_task->erase(t) : ++t;
    }
    //mv new tasks to old maps
    _regular_task->insert(_reserve_task->cbegin(), _reserve_task->cend());
    _reserve_task->clear();
  }

}

void NodeTaskManager::_delete_expired_task() {
  std::unique_lock<mutex> mlock(_del_mtx);
  if (_to_be_del_list.size() > 0) {
    for (const auto &j : _to_be_del_list) {
      _logger->info(
          "job {} is delete by server, now remove it from regular list,", j);
      _regular_task->erase(j);
    }
    _to_be_del_list.clear();
  }

}

void NodeTaskManager::copy_task_running_map() {
  std::time_t now = std::time(nullptr);
  std::unique_lock<mutex> mlock(_run_mtx);
  //copy task object point to running map
  for (auto &task : *_regular_task) {
    if (task.second->reach_time(now)) {
      _running_task->insert(task);
    }
  }
}


size_t NodeTaskManager::running_count() {
  std::unique_lock<mutex> mlock(_regular_mtx);
  return _regular_task->size();
}

TaskManagerInterface* NewTaskManagerPtr() {
  return new NodeTaskManager();
}

} //namespace node

} //namespace webmonitor
