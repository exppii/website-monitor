//
// Created by 刘优 on 16/5/16.
//
#ifndef WEBSITEMONITOR_NODE_GRPC_REMOTE_NODE_H_
#define WEBSITEMONITOR_NODE_GRPC_REMOTE_NODE_H_

#include <memory>

namespace grpc {
  class Channel;
}

namespace webmonitor {

namespace node {

class NodeInterface;

NodeInterface* NewGrpcNodePtr(std::shared_ptr<::grpc::Channel> channel);

}

}




#endif //WEBSITEMONITOR_NODE_GRPC_REMOTE_NODE_H_
