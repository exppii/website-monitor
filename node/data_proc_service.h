//
// Created by 刘优 on 16/6/4.
//

#ifndef WEBSITEMONITOR_NODE_DATA_PROC_SERVICE_H
#define WEBSITEMONITOR_NODE_DATA_PROC_SERVICE_H

#include <string>
#include "common/service_interface.h"

namespace webmonitor {

namespace node {

class DataProcServiceInterface : public ServiceInterface {
public:
  virtual ~DataProcServiceInterface(){}

  virtual bool add_data(const std::string&) = 0;

};

std::unique_ptr<DataProcServiceInterface> DataProcServiceUniquePtr();

} //namespace node
} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_DATA_PROC_SERVICE_H
