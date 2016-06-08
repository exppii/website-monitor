#include <grpc++/grpc++.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/security/server_credentials.h>
#include <iostream>
#include "taskserver/grpc_master_service.h"
#include "common/rpc/async_service_interface.h"
#include "protos/master_service.grpc.pb.h"

#include "common/utils.h"
#include "taskserver/logger.h"

using namespace std;
using namespace webmonitor;


int main(int argc, char const *argv[]) {

  //create logs dir to save log files.
  mkdir_if_not_exists("../logs");

  //init log config
  //node::init_logger(opt.get_log_config_path());
  //test code
  auto outlog = spdlog::stdout_logger_mt(taskserver::SERVER_TAG);
  outlog->set_level(spdlog::level::debug);
  //test code
  auto logger = spdlog::get(taskserver::SERVER_TAG);

  logger->info("current version: 0.1.0");

  logger->info(" init server: 0.0.0.0:50051 ");

  std::string server_address("0.0.0.0:50051");

  //auto builder = std::make_unique<grpc::ServerBuilder>();
  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, ::grpc::InsecureServerCredentials());
  builder.SetMaxMessageSize(std::numeric_limits<int>::max());

  auto master_service = taskserver::GrpcMasterServicePtr(&builder);
  std::unique_ptr<::grpc::Server> server = nullptr;
  server = builder.BuildAndStart();
  master_service->handle_grpc_loop();

  return 0;
}
