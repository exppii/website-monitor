//
// Created by 刘优 on 16/7/5.
//

#include "node/response/ping_response.h"


#include <chrono>

using ms = std::chrono::milliseconds;
using std::chrono::system_clock;
using std::chrono::duration_cast;

namespace webmonitor {

namespace node {

nlohmann::json PingResponse::to_json() const {

  nlohmann::json j;

  j["pktLostPct"] = loss_pct;
  j["avgDurationMSec"] = avg_msec;
  j["minDurationMSec"] = avg_msec;
  j["maxDurationMSec"] = avg_msec;

  j["recvTime"] = duration_cast<ms>(system_clock::now().time_since_epoch()).count();

  return j;
}

} //namespace node

} //namespace webmonitor
