//
// Created by 刘优 on 16/5/12.
//

#ifndef WEBSITEMONITOR_COMMON_RPC_ASYNC_SERVICE_INTERFACE_H_
#define WEBSITEMONITOR_COMMON_RPC_ASYNC_SERVICE_INTERFACE_H_

namespace webmonitor {

// Represents an abstract asynchronous service that handles incoming
// RPCs with a polling loop.
class AsyncServiceInterface {
public:
  virtual ~AsyncServiceInterface() {}

  // A blocking method that should be called to handle incoming RPCs.
  // This method will block until the service shuts down.
  virtual void handle_grpc_loop() = 0;

  // Starts shutting down this service.
  //
  // NOTE(mrry): To shut down this service completely, the caller must
  // also shut down any servers that might share ownership of this
  // service's resources (e.g. completion queues).
  virtual void shutdown() = 0;
};


} // namespace webmonitor


#endif //WEBSITEMONITOR_COMMON_RPC_ASYNC_SERVICE_INTERFACE_H_
