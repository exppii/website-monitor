//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR__NODE_TASK_INTERFACE_H_
#define WEBSITEMONITOR__NODE_TASK_INTERFACE_H_

#include <iostream>
#include <string>

namespace webmonitor {

namespace node {

class TaskInterface {

public:

  virtual ~TaskInterface() { }

  //run task once
  virtual bool run() = 0;

  virtual bool reach_time(const std::time_t&) = 0;


  virtual bool varify_task_content() const = 0;

  virtual bool is_expired() const = 0;

  bool is_running() const { return _running;}

  void set_running_status(const std::time_t& now) {
    _last_run_time = now; _running = true;
  }

protected:

  std::time_t _last_run_time{std::time(nullptr)};
  bool _running{false};

};

}

// inline bool operator==(const TaskInterface &lhs, const TaskInterface &rhs) {
//   return (lhs.equal_to(rhs) && lhs.dump() == rhs.dump());
// }
//
// inline bool operator!=(const TaskInterface &lhs, const TaskInterface &rhs) {
//   return !(lhs == rhs);
// }

} //namespace webmonitor

#endif //WEBSITEMONITOR__NODE_TASK_INTERFACE_H_
