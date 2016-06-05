//
// Created by 刘优 on 16/6/4.
//

#ifndef WEBSITEMONITOR_NODE_SERVER_H_
#define WEBSITEMONITOR_NODE_SERVER_H_

#include <memory>
#include "common/service_interface.h"
namespace webmonitor {

class Options;

namespace node {

class NodeServerInterface : public ServiceInterface {
public:
  virtual ~NodeServerInterface(){}

  virtual int wait_shutdown() = 0;

};

std::unique_ptr<NodeServerInterface> NodeServerUniquePtr(const Options*);

}

}

#endif //WEBSITEMONITOR_NODE_SERVER_H
