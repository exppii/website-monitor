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
#include "taskserver/options.h"

using namespace std;
using namespace webmonitor;


int main(int argc, char const *argv[]) {

  taskserver::Options opt("../conf/taskserver.config");
  //create logs dir to save log files.
  mkdir_if_not_exists("../logs");

  //init log config
  //node::init_logger(opt.get_log_config_path(),taskserver::SERVER_TAG);
  //test code
  auto outlog = spdlog::stdout_logger_mt(taskserver::SERVER_TAG);
  outlog->set_level(spdlog::level::debug);
  //test code
  auto logger = spdlog::get(taskserver::SERVER_TAG);

  logger->info("current version: 0.1.0");

  const std::string server_address =
      opt.get_listen_addr() + ":" + std::to_string(opt.get_listen_port());

  logger->info(" init server: {}", server_address);


  //auto builder = std::make_unique<grpc::ServerBuilder>();
  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, ::grpc::InsecureServerCredentials());
  builder.SetMaxMessageSize(std::numeric_limits<int>::max());

  std::unique_ptr<AsyncServiceInterface> master_service{taskserver::NewGrpcMasterServicePtr(&builder, &opt)};

  std::unique_ptr<::grpc::Server> server = nullptr;
  server = builder.BuildAndStart();
  master_service->handle_grpc_loop();

  return 0;
}
