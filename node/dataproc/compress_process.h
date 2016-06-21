//
// Created by 刘优 on 16/6/20.
//

#ifndef WEBSITEMONITOR_NODE_COMPRESS_PROCESS_H_
#define WEBSITEMONITOR_NODE_COMPRESS_PROCESS_H_

#include <string>

namespace webmonitor {

namespace node {
class DataProcInterface;

DataProcInterface* NewCompressProcPtr(const std::string&);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_COMPRESS_PROCESS_H_
