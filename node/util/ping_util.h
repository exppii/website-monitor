//
// Created by 刘优 on 16/6/27.
//

#ifndef WEBSITEMONITOR_NODE_PING_UTIL_H_
#define WEBSITEMONITOR_NODE_PING_UTIL_H_

#include <string>


namespace webmonitor {

namespace node {

struct PingResponse;

bool ping_ip(const std::string& ip, PingResponse* resp);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_PING_UTIL_H_
