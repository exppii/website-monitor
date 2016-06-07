//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR_NODE_NODE_SERVICE_H_
#define WEBSITEMONITOR_NODE_NODE_SERVICE_H_

#include <memory>
#include "common/service_interface.h"

namespace webmonitor {

class Options;

namespace node {

class TaskManagerInterface;

class DataProcServiceInterface;

std::unique_ptr<ServiceInterface> GrpcServiceUniquePtr(const Options *,
                         DataProcServiceInterface *,
                         TaskManagerInterface *);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_NODE_SERVICE_H_
