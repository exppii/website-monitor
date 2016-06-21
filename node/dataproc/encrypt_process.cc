//
// Created by 刘优 on 16/6/20.
//

#include "node/dataproc/encrypt_process.h"

#include "node/dataproc/data_proc_interface.h"
#include "node/options.h"

namespace webmonitor {

namespace node {

class EncryptProc : public DataProcInterface {
public:
  //TODO
  EncryptProc(const std::string& method){}

  bool proc(nlohmann::json* ) override;

};

//TODO
bool EncryptProc::proc(nlohmann::json* ) {
  return false;
}

DataProcInterface* NewEncryptProcPtr(const std::string& method) {
  return new EncryptProc(method);
}

} //namespace node

} //namespace webmonitor