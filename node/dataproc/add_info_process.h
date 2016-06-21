//
// Created by 刘优 on 16/6/20.
//

#ifndef WEBSITEMONITOR_NODE_ADD_INFO_PROCESS_H_
#define WEBSITEMONITOR_NODE_ADD_INFO_PROCESS_H_

#include <string>
#include "node/dataproc/data_proc_interface.h"

namespace webmonitor {

namespace node {

class Options;

DataProcInterface* NewAddInfoProcPtr(const Options*);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_ADD_INFO_PROCESS_H_
