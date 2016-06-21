//
// Created by 刘优 on 16/6/5.
//

#include "node/dataproc/zmq_process.h"

#include "node/dataproc/data_proc_interface.h"

namespace webmonitor {

namespace node {

class ZMQProc : public DataProcInterface {
public:
  ZMQProc(const std::string& url, const uint32_t port) {

  }

  bool proc(nlohmann::json* ) override ;

};

//TODO
bool ZMQProc::proc(nlohmann::json* data) {

  printf ("zmq will handle the data: %s",data->dump().c_str());
  return true;
}

DataProcInterface* NewZMQProcPtr(const std::string& url, const uint32_t port){
  return new ZMQProc(url, port);
}

} //namespace node
} //namespace webmonitor
