//
// Created by 刘优 on 16/6/27.
//

#ifndef WEBSITEMONITOR_NODE_CURLRESPONSE_H_
#define WEBSITEMONITOR_NODE_CURLRESPONSE_H_

#include "node/response/response_interface.h"

namespace webmonitor {

namespace node {

struct PingResponse : public Response {

  PingResponse(){}

  nlohmann::json to_json() const override;


  double loss_pct{0.0};
  double min_msec{0.0};
  double max_msec{0.0};
  double avg_msec{0.0};

};

} //namespace node

} //namespace webmonitor


#endif
