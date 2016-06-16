//
// Created by 刘优 on 16/6/3.
//

#ifndef WEBSITEMONITOR_NODE_CURLRESPONSE_H_
#define WEBSITEMONITOR_NODE_CURLRESPONSE_H_

#include <math.h>
#include "node/response/response_interface.h"

namespace webmonitor {

namespace node {

class CurlResponse : public Response {

public:
  CurlResponse(){}

  std::string dump() const override;

public:

  void set_respcode(int respcode) {
    _respcode = respcode; }

  void set_curl_code(int curl_code) {
    _curl_code = curl_code; }

  void set_data(const std::string &data) {
    _data = data; }

  void set_total_time(double total_time) {
    _total_time = total_time; }

  void set_namelookup_time(double namelookup_time) {
    _namelookup_time = namelookup_time; }

  void set_connect_time(double connect_time) {
    _connect_time = connect_time; }

  void set_appconnect_time(double appconnect_time) {
    _appconnect_time = appconnect_time; }

  void set_pretransfer_time(double pretransfer_time) {
    _pretransfer_time = pretransfer_time; }

  void set_starttransfer_time(double starttransfer_time) {
    _starttransfer_time = starttransfer_time; }

private:
//in seconds
  std::string _data;
  int _respcode{404};
  int _curl_code{0};
  double _total_time{0.0};
  double _namelookup_time{0.0};
  double _connect_time{0.0};
  double _appconnect_time{0.0};
  double _pretransfer_time{0.0};
  double _starttransfer_time{0.0};

};

}






} //namespace node

#endif //WEBSITEMONITOR_NODE_CURLRESPONSE_H_
