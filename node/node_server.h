//
// Created by 刘优 on 16/6/4.
//

#ifndef WEBSITEMONITOR_NODE_SERVER_H_
#define WEBSITEMONITOR_NODE_SERVER_H_

#include "common/service_interface.h"

namespace webmonitor {

namespace node {

class Options;

class NodeServerInterface : public ServiceInterface {

public:

  virtual ~NodeServerInterface(){}

  virtual int wait_shutdown() = 0;

};

NodeServerInterface* NewNodeServerPtr(const Options*);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_SERVER_H
