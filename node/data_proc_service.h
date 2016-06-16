//
// Created by 刘优 on 16/6/4.
//

#ifndef WEBSITEMONITOR_NODE_DATA_PROC_SERVICE_H
#define WEBSITEMONITOR_NODE_DATA_PROC_SERVICE_H

#include "common/json.hpp"
#include "common/service_interface.h"

namespace webmonitor {

class Options;

namespace node {

class DataProcServiceInterface : public ServiceInterface {
public:
  virtual ~DataProcServiceInterface(){}

  virtual bool add_data(const nlohmann::json& ) = 0;

};

DataProcServiceInterface* NewDataProcServicePtr(const Options*);

} //namespace node
} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_DATA_PROC_SERVICE_H
