//
//  type_safe_queue.h
//  tolly
//
//  Created by 刘优 on 15/9/17.
//  Copyright (c) 2015年 刘优. All rights reserved.
//

#ifndef WEBSITEMONITOR_COMMON_TYPE_SAFE_QUEUE_H_
#define WEBSITEMONITOR_COMMON_TYPE_SAFE_QUEUE_H_


#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>

template <typename T>
class SafeQueue
{
public:
  SafeQueue() {};
  SafeQueue(const SafeQueue&) = delete;            // disable copying
  SafeQueue& operator=(const SafeQueue&) = delete; // disable assignment

  T wait_and_pop()
  {
    std::unique_lock<std::mutex> mlock(_mtx);

    _cond.wait(mlock,[&]{return !_queue.empty();});

    auto val = std::move(_queue.front());
    _queue.pop();
    mlock.unlock();
    return val;
  }

  bool wait_and_pop(T& item,int timeout = 3)
  {
    std::unique_lock<std::mutex> mlock(_mtx);

    auto now = std::chrono::system_clock::now();
    bool rc = false;

    _cond.wait_until(mlock,now + std::chrono::seconds(timeout),[&](){
      rc = !_queue.empty();
      return rc;
    });
    if (!rc) {
      return rc;
    }
    item = std::move(_queue.front());
    _queue.pop();
    return rc;

  }


  bool try_pop(T& item) {

    std::unique_lock<std::mutex> mlock(_mtx);

    if(_queue.empty()) {
      return false;
    }

    item = _queue.front();
    _queue.pop();
    return true;

  }

  void push(const T& item)
  {
    {
      std::unique_lock<std::mutex> mlock(_mtx);

      _queue.push(item);
    }

    _cond.notify_one();
  }

  void push(T&& item)
  {
    {
      std::unique_lock<std::mutex> mlock(_mtx);

      _queue.push(std::forward<T>(item));
    }

    _cond.notify_one();
  }

private:

  std::queue<T> _queue{};
  std::mutex _mtx;
  std::condition_variable _cond;

};

#endif //WEBSITEMONITOR_COMMON_TYPE_SAFE_QUEUE_H_
