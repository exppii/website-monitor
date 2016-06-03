//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITE_MONITOR_INCLUDE_SERVICE_INTERFACE_H_
#define WEBSITE_MONITOR_INCLUDE_SERVICE_INTERFACE_H_

namespace webmonitor {

class ServiceInterface {

public:

  virtual ~ServiceInterface(){}

  virtual void start() = 0;

  virtual void shut_down() = 0;

};

} //namespace webmonitor

#endif //WEBSITE_MONITOR_INCLUDE_SERVICE_INTERFACE_H_
