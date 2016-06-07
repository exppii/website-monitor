//
// Created by 刘优 on 16/6/2.
//

#include "node/grpc_service.h"

#include <thread>
#include <chrono>
#include <map>
#include <vector>
#include <grpc++/grpc++.h>

#include "node/grpc_remote_node.h"
#include "node/task/task_factory.h"
#include "node/logger.h"
#include "node/taskmanager_service.h"

#include "common/options.h"


#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::map;
using std::thread;
using std::chrono::seconds;

namespace webmonitor {

namespace node {

namespace {

struct Info {

  explicit Info(const Options *option) {
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



} //namespace

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

  size_t _parser_task_to_map(const GetJobResponse &resp,
                             std::map<int64_t, TaskSharedPtr> *task_map);


private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};

  bool _running{false};

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

  _logger->info("create insecure grpc channel to task server.");
  _channel = grpc::CreateChannel(url, grpc::InsecureChannelCredentials());

  _grpc_node = GrpcNodePtr(_channel);

}

void GrpcService::start() {
  _logger->info("start GrpcService fetchjob thread...");
  _threads.push_back(thread(&GrpcService::_fetchjob_thread, this));
  _logger->info("start GrpcService report thread...");
  _threads.push_back(thread(&GrpcService::_report_thread, this));

}

void GrpcService::stop() {
  _logger->info("shutdown all GrpcService threads...");
  _running = false;
  for (auto &t : _threads) {
    if (t.joinable()) t.join();
  }

}

void GrpcService::_report_thread() {

  while (_running) {
    _logger->debug("report status to task server.");
    //TODO do report

    std::this_thread::sleep_for(seconds(_info.report_frequency));
  }

}


void GrpcService::_fetchjob_thread() {
  while (_running) {
    _logger->debug("fetch tasks from task server.");
    std::this_thread::sleep_for(seconds(_info.fetch_frequency));
    //TODO do fetch
    GetJobResponse resp{};
    auto status = _grpc_node->get_job(_info.req, &resp);
    if (!status.ok()) {
      _logger->error("fetch job failed: ", status.error_message());
      continue;
    }

    const auto task_maps = std::make_shared<std::map<int64_t, TaskSharedPtr> >();

    if (_parser_task_to_map(resp, task_maps.get()) > 0) {
      _logger->info("fetch {} tasks, now push to taskmanager.",
                     task_maps->size());
      _task_manager->add_task(task_maps);
    }


  }
}

size_t GrpcService::_parser_task_to_map(const GetJobResponse &resp,
                                        std::map<int64_t, TaskSharedPtr> *task_map) {
    TaskFactory factory;
    for(const auto& raw : resp.task_map()) {
      auto task = factory.create(&raw.second);
      if(task) {
        task_map->insert({raw.first,task});
        _logger->debug("success add task: {} to map.", raw.first);
      } else {
        _logger->error("parser task: {} failed. ", raw.first);
      }
    }
    return task_map->size();

}


std::unique_ptr<ServiceInterface> GrpcServiceUniquePtr(
    const TaskManagerInterface *manager, const Options *options) {
  return make_unique<GrpcService>(manager, options);
}

} //namspace node

} //namespace webmonitor
