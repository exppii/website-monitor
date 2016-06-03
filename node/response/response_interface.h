//
// Created by 刘优 on 16/6/3.
//

#ifndef WEBSITEMONITOR_NODE_RESPONSE_INTERFACE_H_
#define WEBSITEMONITOR_NODE_RESPONSE_INTERFACE_H_

#include <string>

namespace webmonitor {

namespace node {

class Response {

public:

  virtual ~Response(){}

  virtual std::string dump() const = 0;

};

}

}



#endif //WEBSITEMONITOR_NODE_RESPONSE_INTERFACE_H_
