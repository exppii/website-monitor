//
// Created by 刘优 on 16/6/5.
//

#ifndef WEBSITEMONITOR_NODE_DATAPROC_ZMQ_PROCESS_H_
#define WEBSITEMONITOR_NODE_DATAPROC_ZMQ_PROCESS_H_

#include <string>

namespace webmonitor {

namespace node {
class DataProcInterface;

DataProcInterface* NewZMQProcPtr(const std::string&, const uint32_t);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_DATAPROC_ZMQ_PROCESS_H_
