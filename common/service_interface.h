//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITE_MONITOR_INCLUDE_SERVICE_INTERFACE_H_
#define WEBSITE_MONITOR_INCLUDE_SERVICE_INTERFACE_H_

namespace webmonitor {

class ServiceInterface {

public:

  ServiceInterface(){}
  ServiceInterface(const ServiceInterface&) = delete;
  ServiceInterface& operator=(const ServiceInterface&) = delete;

  virtual ~ServiceInterface(){}

  virtual void start() = 0;

  virtual void stop() = 0;

};

} //namespace webmonitor

#endif //WEBSITE_MONITOR_INCLUDE_SERVICE_INTERFACE_H_
