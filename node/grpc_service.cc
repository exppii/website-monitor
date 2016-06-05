//
// Created by 刘优 on 16/6/2.
//

#include "node/grpc_service.h"
#include "node/grpc_remote_node.h"

#include "node/logger.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif


namespace webmonitor {

namespace node {

class GrpcService : public ServiceInterface {

public:
  using ServiceInterface::ServiceInterface;

  explicit GrpcService(const TaskManagerInterface* manager):_task_manager(manager) {
    _logger->info("Init Node Service...");
  }

  void start() override;

  void stop() override;

private:

  void _work_thread();

private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};


  const TaskManagerInterface*  _task_manager;

  std::unique_ptr <NodeInterface> _grpc_node;



};

void GrpcService::start() {
  _logger->info("start GrpcService main thread...");
}

void GrpcService::stop() {
  _logger->info("shutdown all GrpcService threads...");
}

std::unique_ptr<ServiceInterface> GrpcServiceUniquePtr(const TaskManagerInterface* manager) {
  return make_unique<GrpcService>(manager);
}


} //namspace node

} //namespace webmonitor
