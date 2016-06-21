//
// Created by 刘优 on 16/6/20.
//

#include "node/dataproc/compress_process.h"

#include "node/dataproc/data_proc_interface.h"
#include "node/options.h"

namespace webmonitor {

namespace node {

class CompressProc : public DataProcInterface {
public:
  //TODO
  CompressProc(const std::string& method){}

  bool proc(nlohmann::json* ) override;

};

//TODO
bool CompressProc::proc(nlohmann::json* ) {
  return false;
}

DataProcInterface* NewCompressProcPtr(const std::string& method) {
  return new CompressProc(method);
}

} //namespace node

} //namespace webmonitor