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

  bool proc(std::string* ) override;

  std::string proc_name() const override { return std::string("compress proc");}
};

//TODO
bool CompressProc::proc(std::string* data) {
  //do nothing now
  printf ("compress do nothing now\n");
  return true;
}

DataProcInterface* NewCompressProcPtr(const std::string& method) {
  return new CompressProc(method);
}

} //namespace node

} //namespace webmonitor
