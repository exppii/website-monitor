//
// Created by 刘优 on 16/6/5.
//

#ifndef WEBSITEMONITOR_NODE_DATAPROCINTERFACE_H_
#define WEBSITEMONITOR_NODE_DATAPROCINTERFACE_H_


#include <memory>
#include "common/json.hpp"

namespace webmonitor {

namespace node {

class DataProcInterface {

public:

  virtual ~DataProcInterface(){}

  virtual bool proc(nlohmann::json* ) = 0;

};

using DataProcInterfacePtr = std::unique_ptr<DataProcInterface>;

} //namespace node
} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_DATAPROCINTERFACE_H_
