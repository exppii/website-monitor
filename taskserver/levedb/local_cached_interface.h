//
// Created by 刘优 on 16/6/8.
//

#ifndef WEBSITEMONITOR_TASKSERVER_LOCAL_CACHED_INTERFACE_H
#define WEBSITEMONITOR_TASKSERVER_LOCAL_CACHED_INTERFACE_H

namespace webmonitor {

namespace taskserver {

class LocalCachedInterface {

public:

  LocalCachedInterface(){}
  //disable copy asign and move constructor
  LocalCachedInterface(const LocalCachedInterface&) = delete;
  LocalCachedInterface& operator=(const LocalCachedInterface&) = delete;
  LocalCachedInterface(LocalCachedInterface&&) = delete;

  virtual ~LocalCachedInterface(){}

  virtual add_tasks(int64_t id, )

};

} //namespace taskserver

} //namspace webmonitor

#endif //WEBSITEMONITOR_TASKSERVER_LOCAL_CACHED_INTERFACE_H
