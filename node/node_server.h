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

std::unique_ptr<ServiceInterface> NodeServerUniquePtr(const Options*);

}

}

#endif //WEBSITEMONITOR_NODE_SERVER_H
