//
// Created by 刘优 on 16/6/4.
//

#include "node/node_server.h"
#include "node/logger.h"

#include "common/options.h"

#include "node/data_proc_service.h"
#include "node/taskmanager_service.h"
#include "node/grpc_service.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

using std::unique_ptr;

namespace webmonitor {

namespace node {

class NodeServer : public ServiceInterface {

public:
  using ServiceInterface::ServiceInterface;

  NodeServer(const Options* options){
    _logger->info("Init NodeServer...");
  }

  void start() override;

  void stop() override;

private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};

  std::unique_ptr<DataProcServiceInterface> _data_proc;
  std::unique_ptr<TaskManagerInterface> _task_manager{TaskManagerUniquePtr(_data_proc.get())};
  std::unique_ptr<ServiceInterface> _grpc_service{GrpcServiceUniquePtr(_task_manager.get())};

};

void NodeServer::start() {
  _logger->info("start NodeServer main thread...");
}

void NodeServer::stop() {
  _logger->info("shutdown all NodeServer threads...");
}


std::unique_ptr<ServiceInterface> NodeServerUniquePtr(const Options* options) {
  return make_unique<NodeServer>(options);
}

} //namspace node

} //namespace webmonitor
