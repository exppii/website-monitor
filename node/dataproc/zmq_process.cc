//
// Created by 刘优 on 16/6/5.
//

#include "node/dataproc/zmq_process.h"
#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

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

DataProcInterfacePtr ZMQProcUniuePtr(const std::string& url, const uint32_t port){
  return make_unique<ZMQProc>(url, port);
}

} //namespace node
} //namespace webmonitor
