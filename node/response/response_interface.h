//
// Created by 刘优 on 16/6/3.
//

#ifndef WEBSITEMONITOR_NODE_RESPONSE_INTERFACE_H_
#define WEBSITEMONITOR_NODE_RESPONSE_INTERFACE_H_

#include "common/json.hpp"

namespace webmonitor {

namespace node {

class Response {

public:

  virtual ~Response(){}

  virtual nlohmann::json to_json() const = 0;

};

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_RESPONSE_INTERFACE_H_
