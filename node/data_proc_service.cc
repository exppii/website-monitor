//
// Created by 刘优 on 16/6/4.
//

#include "node/data_proc_service.h"

#include <thread>

#include "node/logger.h"
#include "node/dataproc/zmq_process.h"


#include "common/options.h"
#include "common/type_safe_queue.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

using std::thread;
using std::string;
using std::unique_ptr;
using std::chrono::seconds;

namespace webmonitor {

namespace node {

class NodeDataProcService : public DataProcServiceInterface {
public:

  explicit NodeDataProcService(const Options* option) {

      //_proc_list.push_back(CompressProcUniuePtr());
      _proc_list.push_back(ZMQProcUniuePtr(option->get_upload_addr(),option->get_upload_port()));
  }

  bool add_data(const std::string& data) override;

  void start() override ;

  void stop() override ;

private:

  void _work_thread();

private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};

  bool _running{true};

  unique_ptr<thread> _thread{nullptr};
  SafeQueue<string> _queue;

  std::vector<DataProcInterfacePtr> _proc_list;

};

bool NodeDataProcService::add_data(const std::string & data) {
  _logger->debug("push one data to queue...");
  _queue.push(data);
  return true;
}

void NodeDataProcService::start() {
  _logger->info("start NodeDataProcService main thread...");

  _thread.reset(new thread(&NodeDataProcService::_work_thread,this));
}

void NodeDataProcService::stop() {
  if(_thread && _thread->joinable()) {
    _thread->join();
  }
  _logger->debug("data proc thread stopped.");
}

void NodeDataProcService::_work_thread() {
  _logger->debug("data proc thread started.");
  while (_running) {
    std::string data{};
    if(_queue.wait_and_pop(data,3)) {
      for (const auto& proc : _proc_list) proc->proc(&data);
    }
  }
}


std::unique_ptr<DataProcServiceInterface> DataProcServiceUniquePtr(const Options* options) {
  return make_unique<NodeDataProcService>(options);
}
} //namespace node

} //namespace webmonitor