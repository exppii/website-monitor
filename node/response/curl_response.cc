//
// Created by 刘优 on 16/6/3.
//

#include "node/response/curl_response.h"


namespace webmonitor {

namespace node {

nlohmann::json node::CurlResponse::to_json() const {

  nlohmann::json j;

  j["respcode"] = _respcode;
  j["curl_code"] = _curl_code;
  j["data"] = _data;

  j["total_time"] = _total_time;
  j["namelookup_time"] = _namelookup_time;
  j["connect_time"] = _connect_time;
  j["appconnect_time"] = _appconnect_time;
  j["pretransfer_time"] = _pretransfer_time;
  j["starttransfer_time"] = _starttransfer_time;
  return j;
}

}

}
