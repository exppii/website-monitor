//
// Created by 刘优 on 16/6/3.
//

#include "node/response/curl_response.h"
#include <chrono>

using ms = std::chrono::milliseconds;
using std::chrono::system_clock;
using std::chrono::duration_cast;

namespace webmonitor {

namespace node {

nlohmann::json node::CurlResponse::to_json() const {

  nlohmann::json j;

  j["respcode"] = respcode;
  j["curl_code"] = curl_code;
  j["data"] = data;

  j["total_time"] = total_time;
  j["namelookup_time"] = namelookup_time;
  j["connect_time"] = connect_time;
  j["appconnect_time"] = appconnect_time;
  j["pretransfer_time"] = pretransfer_time;
  j["starttransfer_time"] = starttransfer_time;

  j["recvTime"] = duration_cast<ms>(system_clock::now().time_since_epoch()).count();
  return j;
}

}

}
