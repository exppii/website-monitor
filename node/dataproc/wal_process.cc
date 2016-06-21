//
// Created by 刘优 on 16/6/20.
//

#include "node/dataproc/wal_process.h"

#include "node/dataproc/data_proc_interface.h"
#include "node/options.h"

namespace webmonitor {

namespace node {

class WALProc : public DataProcInterface {
public:
  //TODO
  WALProc(const std::string& path){}

  bool proc(nlohmann::json* ) override;

};

//TODO
bool WALProc::proc(nlohmann::json* ) {
  return false;
}

DataProcInterface* NewWALProcPtr(const std::string& path) {
  return new WALProc(path);
}

} //namespace node

} //namespace webmonitor
