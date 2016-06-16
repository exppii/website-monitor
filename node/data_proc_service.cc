//
// Created by 刘优 on 16/6/4.
//

#include "node/data_proc_service.h"

#include <thread>

#include "node/logger.h"
#include "node/dataproc/zmq_process.h"


#include "node/options.h"
#include "common/type_safe_queue.h"


using std::thread;
using std::string;
using std::unique_ptr;
using std::chrono::seconds;

namespace webmonitor {

namespace node {

class NodeDataProcService : public DataProcServiceInterface {
public:

  using DataProcInterfacePtr = std::unique_ptr<DataProcInterface>;

  explicit NodeDataProcService(const Options* option) {
      _logger->info("Init data process service...");
      //_proc_list.push_back(CompressProcUniuePtr());
      _logger->info("push ZMQ proc handle to proc list.");
      DataProcInterfacePtr zmq(NewZMQProcPtr(option->get_upload_addr(),option->get_upload_port()));
      _proc_list.push_back(std::move(zmq));

      _logger->debug("finined init data process service.");
  }

  bool add_data(const nlohmann::json& data) override;

  void start() override ;

  void stop() override ;

private:

  void _work_thread();

private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};

  bool _running{true};

  unique_ptr<thread> _thread{nullptr};
  SafeQueue<nlohmann::json> _queue;

  std::vector<DataProcInterfacePtr> _proc_list;

};

bool NodeDataProcService::add_data(const nlohmann::json& data) {
  _logger->debug("push one data to queue...");
  _queue.push(data);
  return true;
}

void NodeDataProcService::start() {
  _logger->info("start DataProcService work thread...");
  _running = true;
  _thread.reset(new thread(&NodeDataProcService::_work_thread,this));
}

void NodeDataProcService::stop() {
  _running = false;
  if(_thread && _thread->joinable()) {
    _thread->join();
  }
  _logger->debug("data proc thread stopped.");
}

void NodeDataProcService::_work_thread() {
  _logger->debug("data proc thread started.");
  while (_running) {
    nlohmann::json data{};
    if(_queue.wait_and_pop(data,3)) {
      for (const auto& proc : _proc_list) proc->proc(&data);
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
