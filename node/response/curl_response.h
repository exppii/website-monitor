//
// Created by 刘优 on 16/6/3.
//

#ifndef WEBSITEMONITOR_NODE_CURLRESPONSE_H_
#define WEBSITEMONITOR_NODE_CURLRESPONSE_H_


#include "node/response/response_interface.h"

namespace webmonitor {

namespace node {

struct CurlResponse : public Response {

public:
  CurlResponse(){}

  nlohmann::json to_json() const override;

//in seconds
  std::string data;
  int respcode{404};
  int curl_code{0};
  double total_time{0.0};
  double namelookup_time{0.0};
  double connect_time{0.0};
  double appconnect_time{0.0};
  double pretransfer_time{0.0};
  double starttransfer_time{0.0};

};

}






} //namespace node

#endif //WEBSITEMONITOR_NODE_CURLRESPONSE_H_
