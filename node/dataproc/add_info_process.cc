//
// Created by 刘优 on 16/6/20.
//

#include "node/dataproc/add_info_process.h"

#include "node/dataproc/data_proc_interface.h"
#include "node/options.h"

namespace webmonitor {

namespace node {

class AddInfoProc : public DataProcInterface {
public:
  //TODO
  AddInfoProc(const Options* option){}

  bool proc(nlohmann::json* ) override;

};

//TODO
bool AddInfoProc::proc(nlohmann::json* ) {
  return false;
}

DataProcInterface* NewAddInfoProcPtr(const Options* option) {
  return new AddInfoProc(option);
}

} //namespace node

} //namespace webmonitor