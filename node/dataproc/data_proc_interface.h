//
// Created by 刘优 on 16/6/5.
//

#ifndef WEBSITEMONITOR_NODE_DATAPROCINTERFACE_H_
#define WEBSITEMONITOR_NODE_DATAPROCINTERFACE_H_


#include <string>

namespace webmonitor {

namespace node {

class DataProcInterface {

public:

  virtual ~DataProcInterface(){}

  virtual bool proc(std::string* ) = 0;

  virtual std::string proc_name() const = 0;

};

} //namespace node
} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_DATAPROCINTERFACE_H_
