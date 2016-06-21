//
// Created by 刘优 on 16/6/21.
//

#ifndef WEBSITEMONITOR_NODE_PROCESS_LIB_H_
#define WEBSITEMONITOR_NODE_PROCESS_LIB_H_

#include <vector>
#include <memory>
#include "node/dataproc/data_proc_interface.h"


namespace webmonitor {

namespace node {

class Options;

using DataProcInterfacePtr = std::unique_ptr<DataProcInterface>;

void create_data_proc_list(const Options*, std::vector<DataProcInterfacePtr>* );

DataProcInterface* create_data_sender(const Options*);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_PROCESS_LIB_H_
