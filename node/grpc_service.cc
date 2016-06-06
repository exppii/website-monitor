//
// Created by 刘优 on 16/6/2.
//

#include "node/grpc_service.h"

#include <thread>
#include <chrono>
#include <grpc++/grpc++.h>

#include "node/grpc_remote_node.h"
#include "node/logger.h"

#include "common/options.h"
#include "common/type_safe_queue.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::thread;
using std::chrono::seconds;

namespace webmonitor {

namespace node {


namespace {

struct Info {

  explicit Info(const Options* option) {
    fetch_frequency = option->get_fetchtask_frequency() * 60;
    report_frequency = option->get_report_frequency() * 60;

    auto info = req.mutable_node();

    info->set_id(option->get_node_id());
    info->set_name(option->get_node_name());

  }

  int fetch_frequency{60}; //in seconds
  int report_frequency{180}; //in seconds
  GetJobRequest req{};
};

}


class GrpcService : public ServiceInterface {

public:
  using ServiceInterface::ServiceInterface;

  explicit GrpcService(const TaskManagerInterface *manager,
                       const Options *options);

  void start() override;

  void stop() override;

private:

  void _fetchjob_thread();

  void _report_thread();

private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};

  bool _running {false};

  vector<thread> _threads{};

  const TaskManagerInterface *_task_manager;

  Info _info;

  shared_ptr<::grpc::Channel> _channel{nullptr};

  std::unique_ptr<NodeInterface> _grpc_node{nullptr};



};

GrpcService::GrpcService(const TaskManagerInterface *manager,
                         const Options *options)
    : _task_manager(manager), _info(options) {
  _logger->info("Init Node Service...");

  const auto url = options->get_taskserver_addr() +
                   std::to_string(options->get_taskserver_port());

  _channel = grpc::CreateChannel(url, grpc::InsecureChannelCredentials());

  _grpc_node = GrpcNodePtr(_channel);



}

void GrpcService::start() {
  _logger->info("start GrpcService main thread...");

  _threads.push_back(thread(&GrpcService::_fetchjob_thread,this));
  _threads.push_back(thread(&GrpcService::_report_thread,this));

}

void GrpcService::stop() {
  _logger->info("shutdown all GrpcService threads...");
  _running = false;
  for (auto& t : _threads) {
    if(t.joinable()) t.join();
  }

}

void GrpcService::_report_thread() {

 while (_running) {

   //TODO do report

   std::this_thread::sleep_for(seconds(_info.report_frequency));
 }

}


void GrpcService::_fetchjob_thread() {
  while (_running) {

    //TODO do fetch

    std::this_thread::sleep_for(seconds(_info.fetch_frequency));
  }
}




std::unique_ptr<ServiceInterface> GrpcServiceUniquePtr(
    const TaskManagerInterface *manager, const Options *options) {
  return make_unique<GrpcService>(manager, options);
}


} //namspace node

} //namespace webmonitor
