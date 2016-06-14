//
// Created by 刘优 on 16/5/12.
//

#ifndef WEBSITEMONITOR_WEBCLIENT_GRPC_REMOTE_MASTER_H_
#define WEBSITEMONITOR_WEBCLIENT_GRPC_REMOTE_MASTER_H_

#include "cwebclient/master_interface.h"

namespace grpc {
  class Channel;
}

namespace webmonitor {

namespace webclient {

std::unique_ptr <MasterInterface> GrpcMasterPtr(
    std::shared_ptr<::grpc::Channel> channel);

} //namespace webclient

} //namespace webmonitor

#endif //WEBSITEMONITOR_WEBCLIENT_GRPC_REMOTE_MASTER_H_
