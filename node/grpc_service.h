//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR_NODE_SERVICE_H_
#define WEBSITEMONITOR_NODE_SERVICE_H_

#include <memory>

namespace webmonitor {

class Options;

class ServiceInterface;

namespace node {

class TaskManagerInterface;

class DataProcServiceInterface;

ServiceInterface *NewGrpcServicePtr(const Options *,
                                    std::shared_ptr<DataProcServiceInterface>,
                                    std::shared_ptr<TaskManagerInterface>);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_SERVICE_H_
