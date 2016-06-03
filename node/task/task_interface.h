//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR__NODE_TASK_INTERFACE_H_
#define WEBSITEMONITOR__NODE_TASK_INTERFACE_H_

#include <iostream>
#include <string>

namespace webmonitor {

class TaskInterface {

public:

  TaskInterface(const int64_t &id,
                const int &frequency,
                const std::string &dest)
      : _ID(id), _frequency(frequency), _dest(dest) { }

  virtual ~TaskInterface() { }

  //dump inheritance object values
  virtual std::string dump() const = 0;

  //run task once
  virtual void run() = 0;

  bool varify_task_content() const { return _task_content_ok; }
  bool equal_to(const TaskInterface &rhs) const {
    return (_ID == rhs._ID &&
           _frequency == rhs._frequency &&
           _dest == rhs._dest);
  }
protected:

  const int64_t _ID{0};
  int _frequency{10};
  std::string _dest{""};

  bool _task_content_ok{true};

};

inline bool operator==(const TaskInterface &lhs, const TaskInterface &rhs) {
  return (lhs.equal_to(rhs) && lhs.dump() == rhs.dump());
}

inline bool operator!=(const TaskInterface &lhs, const TaskInterface &rhs) {
  return !(lhs == rhs);
}

} //namespace webmonitor

#endif //WEBSITEMONITOR__NODE_TASK_INTERFACE_H_
