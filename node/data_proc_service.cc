//
// Created by 刘优 on 16/6/4.
//

#include "node/data_proc_service.h"

#include <thread>

#include "node/logger.h"
#include "node/dataproc/process_lib.h"


#include "node/options.h"
#include "common/type_safe_queue.h"


using std::thread;
using std::string;
using std::unique_ptr;
using std::vector;
using std::chrono::seconds;

namespace webmonitor {

namespace node {

class NodeDataProcService : public DataProcServiceInterface {
public:

  explicit NodeDataProcService(const Options* options);

  bool add_data(const nlohmann::json& data) override;

  void start() override ;

  void stop() override ;

private:

  void _data_pre_proc_thread();

  void _send_to_server_thread();

private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};

  bool _running{true};

  unique_ptr<thread> _thread{nullptr};
  vector<thread> _threads;

  SafeQueue<nlohmann::json> _queue;

  std::vector<DataProcInterfacePtr> _pre_proc_list{};
  DataProcInterfacePtr _data_sender{nullptr};

};

NodeDataProcService::NodeDataProcService(const Options* options) {

  create_data_proc_list(options,&_pre_proc_list);

  _data_sender.reset(create_data_sender(options));

  _logger->debug("finined init data process service.");
}

bool NodeDataProcService::add_data(const nlohmann::json& data) {
  _logger->debug("push one data to queue...");
  _queue.push(data);
  return true;
}

void NodeDataProcService::start() {
  _logger->info("start DataProcService work thread...");
  _running = true;
  _threads.push_back(thread(&NodeDataProcService::_data_pre_proc_thread,this));
  _threads.push_back(thread(&NodeDataProcService::_send_to_server_thread,this));
}

void NodeDataProcService::stop() {
  _running = false;
  for(auto& t : _threads) {
    if(t.joinable()) t.join();
  }

  _logger->debug("data proc thread stopped.");
}

void NodeDataProcService::_data_pre_proc_thread() {
  _logger->debug("data pre proc thread started.");
  while (_running) {
    nlohmann::json data{};
    if(_queue.wait_and_pop(data,3)) {
      for (const auto& proc : _pre_proc_list) proc->proc(&data);
    } else {
      _logger->debug("no data in proc service...");
    }
  }
}

//TODO send to zmq server
void NodeDataProcService::_send_to_server_thread() {
  _logger->debug("data sender thread started.");
  while (_running) {
    nlohmann::json data{};
    if(_queue.wait_and_pop(data,3)) {
      for (const auto& proc : _pre_proc_list) proc->proc(&data);
    } else {
      _logger->debug("no data in proc service...");
    }
  }
}


DataProcServiceInterface* NewDataProcServicePtr(const Options* options) {
  return new NodeDataProcService(options);
}
} //namespace node

} //namespace webmonitor
