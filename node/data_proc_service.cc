//
// Created by 刘优 on 16/6/4.
//

#include "node/data_proc_service.h"

#include <thread>

#include "common/options.h"
#include "node/logger.h"

#include "node/dataproc/zmq_process.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

using std::thread;
using std::unique_ptr;

using std::chrono::seconds;


namespace webmonitor {

namespace node {

class NodeDataProcService : public DataProcServiceInterface {
public:

  explicit NodeDataProcService(const Options* option)
      : _upaddr(option->get_upload_addr()),
        _port(option->get_upload_port()){

      _proc_list.push_back(ZMQProcUniuePtr(option->get_upload_addr(),option->get_upload_port()));

  }

  bool add_data(const std::string& data) override;

  void start() override ;

  void stop() override ;

private:

  void _work_thread();

private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};
  const std::string _upaddr;
  const int _port;

  bool _running{true};
  unique_ptr<thread> _thread{nullptr};

  std::vector<DataProcInterfacePtr> _proc_list;

};

bool NodeDataProcService::add_data(const std::string & data) {

  return false;
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

    std::this_thread::sleep_for(seconds(3));
  }
}


std::unique_ptr<DataProcServiceInterface> DataProcServiceUniquePtr(const Options* options) {
  return make_unique<NodeDataProcService>(options);
}
} //namespace node

} //namespace webmonitor