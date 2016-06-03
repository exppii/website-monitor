//
//  task_cache.h
//  TaskCache
//
//  Created by 刘优 on 16/5/3.
//  Copyright © 2016年 刘优. All rights reserved.
//

#ifndef WEBSITEMONITOR_TASK_CACHE_H
#define WEBSITEMONITOR_TASK_CACHE_H

#include <string>
#include <vector>
#include <time.h>
#include <mutex>
#include <memory>
#include <map>
#include <queue>
#include <iostream>


template<typename T>
class TaskCache {
  
public:

  TaskCache(){}
  
  //disable copy asign and move constructor
  TaskCache(const TaskCache&) = delete;
  TaskCache& operator=(const TaskCache&) = delete;
  TaskCache(TaskCache&&) = delete;
  
  ~TaskCache(){}
  
public:
  
  bool add(const T& task);
  bool add(const std::vector<T>& tasks);
  bool del(const T& task);
  bool update(const T& task);
  bool get(T& task);

#ifndef NDEBUG
  //for test
  void print_tasks_status(const std::string& info);
#endif
  
protected:
  
  void _safe_add_to_candidate(const std::shared_ptr<T> task);
  void _safe_add_to_map(const std::shared_ptr<T> task);
  
private:
  
  std::map<long long, std::shared_ptr<T> > _task_maps;
  std::queue<std::shared_ptr<T>> _task_queue;
  std::mutex _map_mtx,_que_mtx;
  
};

template<typename T>
bool TaskCache<T>::add(const T &task) {
  std::unique_lock<std::mutex> mlock(_que_mtx);
  _task_queue.push(std::make_shared<T>(task));
  std::cout << "add task: " << task.id() << " to cached map. " << std::endl;
  return true;
}

template<typename T>
bool TaskCache<T>::add(const std::vector<T> &tasks){
  
  std::unique_lock<std::mutex> mlock(_que_mtx);
  for (const T& t : tasks) {
    _task_queue.push(std::make_shared<T>(t));
  }
  return true;
  
}

template<typename T>
bool TaskCache<T>::get(T& task) {
  
  std::unique_lock<std::mutex> l1(_que_mtx);
  
  if(_task_queue.empty()) return false;
  
  const auto t = _task_queue.front();
  task = *t;
  
  //add task to map
  _safe_add_to_map(t);
  
  _task_queue.pop();
  
  return true;
}

template<typename T>
bool TaskCache<T>::update(const T& task) {
  
  auto id = task.id();
  std::cout << "being update id: " << id << std::endl;
  std::unique_lock<std::mutex> mlock(_map_mtx);
  
  auto iter = _task_maps.find(id);

  if(iter != _task_maps.end()) {
    _task_maps[id].reset(new T(task));
    std::cout << "update task: " << task.id() << " to cached map. " << std::endl;
    return true;
  }
  std::cout << "did not find task: " << task.id() << " in cached map. " << std::endl;
//  if(iter != _task_maps.end() ) {
//    if(task.get_status() != TASKSTATUS::FAILED)
//      _task_maps[id].reset(new T(task));
//    else {
//      _safe_add_to_candidate(iter->second);
//      _task_maps.erase(iter);
//    }
//    return true;
//  }
  return false;
  
}

template<typename T>
bool TaskCache<T>::del(const T &task) {
  auto id = task.id();

  std::unique_lock<std::mutex> mlock(_map_mtx);

  auto iter = _task_maps.find(id);

  if(iter != _task_maps.end()) {
    _task_maps.erase(iter);
    return true;
  }
  return false;
}

template<typename T>
void TaskCache<T>::_safe_add_to_candidate(const std::shared_ptr<T> task) {
  std::unique_lock<std::mutex> l1(_que_mtx);
  _task_queue.push(task);
}

template<typename T>
void TaskCache<T>::_safe_add_to_map(const std::shared_ptr<T> task) {
  std::unique_lock<std::mutex> mlock(_map_mtx);
  _task_maps[task->get_id()] = task;
  
}

#ifndef NDEBUG
//for test
template<typename T>
void TaskCache<T>::print_tasks_status(const std::string& info) {
  
  std::cout << info << std::endl;
  {
    
    std::unique_lock<std::mutex> mlock(_map_mtx);
    for(const auto& p: _task_maps) {
      std::cout << "task: " << p.first << " status: " <<  static_cast<int>(p.second->get_status()) << std::endl;
    }
  }
  {
    std::unique_lock<std::mutex> mlock(_que_mtx);
    std::cout << "candidate task size:" << _task_queue.size()<< std::endl;
    
  }
  
}
#endif
#endif /* task_manager_hpp */
