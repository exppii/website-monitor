//
// Created by 刘优 on 16/6/3.
//

#include "node/response/curl_response.h"
#include "common/json.hpp"

namespace webmonitor {

namespace node {

std::string node::CurlResponse::dump() const {

  nlohmann::json j;

  j["respcde"] = _respcode;
  j["redirect"] = _redirect;
  j["url"] = _url;

  j["total_time"] = _total_time;
  j["namelookup_time"] = _namelookup_time;
  j["connect_time"] = _connect_time;
  j["appconnect_time"] = _appconnect_time;
  j["pretransfer_time"] = _pretransfer_time;
  j["starttransfer_time"] = _starttransfer_time;
  return j.dump();
}

}

}
