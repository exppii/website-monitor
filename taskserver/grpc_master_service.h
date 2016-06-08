//
// Created by 刘优 on 16/5/12.
//

#ifndef WEBSITEMONITOR_TASKSERVER_GRPC_MASTER_SERVICE_H_
#define WEBSITEMONITOR_TASKSERVER_GRPC_MASTER_SERVICE_H_


#include "common/rpc/async_service_interface.h"
#include <memory>

namespace grpc {
class ServerBuilder;
}

namespace webmonitor {

namespace taskserver {

std::unique_ptr <AsyncServiceInterface> GrpcMasterServicePtr(
    ::grpc::ServerBuilder *builder);

} //namespace taskserver

} //namespace webmonitor

#endif //WEBSITEMONITOR_TASKSERVER_GRPC_MASTER_SERVICE_H_
