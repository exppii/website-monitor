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
#include "node/data_proc_service.h"
#include "node/options.h"
#include "node/node_interface.h"

#include "protos/master_service.pb.h"


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
    fetch_frequency = option->get_fetchtask_frequency() * 6;
    report_frequency = option->get_report_frequency() * 6;
    req.set_node_id(option->get_node_id());
  }

  uint32_t fetch_frequency{60}; //in seconds
  uint32_t report_frequency{180}; //in seconds
  GetJobRequest req{};
};


} //namespace

class GrpcService : public ServiceInterface {

public:
  using ServiceInterface::ServiceInterface;

  explicit GrpcService(const Options *, std::shared_ptr<TaskManagerInterface>);

  void start() override;

  void stop() override;

private:

  void _fetchjob_thread();

  void _report_thread();

  size_t _parser_task_to_map(const GetJobResponse &resp,
                             std::map<uint64_t, TaskSharedPtr> *task_map);


private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};

  bool _running{true};

  vector<thread> _threads{};

  Info _info;

  std::shared_ptr<TaskManagerInterface> _task_manager;

  shared_ptr<::grpc::Channel> _channel{nullptr};

  std::unique_ptr<NodeInterface> _grpc_node{nullptr};

};

GrpcService::GrpcService(const Options *options,
                          std::shared_ptr<TaskManagerInterface> manager)
    : _info(options), _task_manager(manager) {
  _logger->info("Init grpc service...");
  const auto url = options->get_taskserver_addr() + ":" +
                   std::to_string(options->get_taskserver_port());

  _logger->info("Create insecure grpc channel to task server: {}. ", url);
  _channel = grpc::CreateChannel(url, grpc::InsecureChannelCredentials());

  _grpc_node.reset(NewGrpcNodePtr(_channel));

}

void GrpcService::start() {
  _running = true;
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
  _logger->info("Current report frequency: {}", _info.report_frequency);
  while (_running) {
    _logger->debug("report status to task server.");
    //TODO do report

    std::this_thread::sleep_for(seconds(_info.report_frequency));
  }

}


void GrpcService::_fetchjob_thread() {
  _logger->info("current fetch frequency: {}. ", _info.fetch_frequency);
  while (_running) {

    std::this_thread::sleep_for(seconds(_info.fetch_frequency));
    _logger->debug("fetch tasks from task server.");
    //TODO do fetch
    GetJobResponse resp{};
    _info.req.set_running_task_count(_task_manager->running_count());
    auto status = _grpc_node->get_job(_info.req, &resp);
    if (!status.ok()) {
      _logger->error("Fetch task failed, error code: {} ,error message: {} ",
                     status.error_code(), status.error_message());
      //TODO handle the error
      continue;
    }

    std::vector<uint64_t> droplist(resp.dropped_list().cbegin(),
                                   resp.dropped_list().cend());
    if (droplist.size() > 0) {
      _logger->info(
          "get drop list from server. now push the list to taskmanager.");
      _task_manager->del_task(droplist);
    }

    std::map<uint64_t, TaskSharedPtr> task_maps{};

    if (_parser_task_to_map(resp, &task_maps) > 0) {
      _logger->info("fetch {} tasks, now push to taskmanager.",
                    task_maps.size());
      _task_manager->add_task(task_maps);
    }


  }
}

size_t GrpcService::_parser_task_to_map(const GetJobResponse &resp,
                                        std::map<uint64_t, TaskSharedPtr> *task_map) {
  TaskFactory factory;
  for (const auto &raw : resp.task_map()) {
    auto task = factory.create(&raw.second);
    if (task) {
      task_map->insert({raw.first, task});
      _logger->debug("success add task: {} to map.", raw.first);
    } else {
      _logger->error("parser task: {} failed. ", raw.first);
    }
  }
  return task_map->size();

}


ServiceInterface *NewGrpcServicePtr(const Options *options,
                                    std::shared_ptr<TaskManagerInterface> manager) {
  return new GrpcService(options, manager);
}

} //namspace node

} //namespace webmonitor
