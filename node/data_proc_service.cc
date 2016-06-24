//
// Created by 刘优 on 16/6/4.
//

#include "node/data_proc_service.h"

#include <thread>

#include "node/logger.h"
#include "node/dataproc/process_lib.h"
#include "node/local_cache_util.h"


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

  void _write_to_local_thread();

private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(node::NODE_TAG)};

  const uint64_t _NODE_ID;

  bool _running{true};

  unique_ptr<thread> _thread{nullptr};

  unique_ptr<LocalCachedInterface> _cached;

  std::vector<DataProcInterfacePtr> _pre_proc_list{};

  vector<thread> _threads;

  SafeQueue<nlohmann::json> _queue;




};

NodeDataProcService::NodeDataProcService(const Options* options)
  :_NODE_ID(options->get_node_id()),
  _cached(NewLevelDBCachedPtr(options)) {

  create_data_proc_list(options,&_pre_proc_list);

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
  _threads.push_back(thread(&NodeDataProcService::_write_to_local_thread,this));
}

void NodeDataProcService::stop() {
  _running = false;
  for(auto& t : _threads) {
    if(t.joinable()) t.join();
  }

  _logger->debug("data proc thread stopped.");
}

void NodeDataProcService::_write_to_local_thread() {
  _logger->debug("write to local thread started.");
  while (_running) {
    nlohmann::json data{};
    if(_queue.wait_and_pop(data,3)) {
      //add base info to data
      data["node_id"] = _NODE_ID;
      _logger->debug("add data to local cache.");
      _cached->add(data.dump());
    } else {
      _logger->debug("no data in proc service...");
    }
  }
}

//TODO send to zmq server
void NodeDataProcService::_data_pre_proc_thread() {

  _logger->debug("data pre proc thread started.");

  while (_running) {
    std::string data;
    if(_cached->get(&data)) {
      _logger->debug("get data to from cache: {}", data);
      bool proc_ret = true;
      for (const auto& proc : _pre_proc_list) {
        if(!proc->proc(&data)) {
          proc_ret = false;
          _logger->error("{} meet error!!! ", proc->proc_name());
          break;
        }
      }
      //proc_ret ? _cached->del_last_get(): _cached->recovery();
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}


DataProcServiceInterface* NewDataProcServicePtr(const Options* options) {
  return new NodeDataProcService(options);
}
} //namespace node

} //namespace webmonitor
