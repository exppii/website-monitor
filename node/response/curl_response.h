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
  int get_respcode() const {
    return _respcode;
  }

  void set_respcode(int respcode) {
    _respcode = respcode;
  }

  int get_redirect() const {
    return _redirect;
  }

  void set_redirect(int redirect) {
    _redirect = redirect;
  }

  std::string get_url() const {
    return _url;
  }

  void set_url(const char* url) {
    _url = std::string("") + url;
  }

private:
  std::string _url;
public:
  void set_data(const std::string &data) {
    _data = data;
  }

private:
  std::string _data;
  int _respcode{404};
  int _redirect;

public:
  void set_total_time(double total_time) {
    _total_time = total_time;
  }

  void set_namelookup_time(double namelookup_time) {
    _namelookup_time = namelookup_time;
  }

  void set_connect_time(double connect_time) {
    _connect_time = connect_time;
  }

  void set_appconnect_time(double appconnect_time) {
    _appconnect_time = appconnect_time;
  }

  void set_pretransfer_time(double pretransfer_time) {
    _pretransfer_time = pretransfer_time;
  }

  void set_starttransfer_time(double _starttransfer_time) {
    CurlResponse::_starttransfer_time = _starttransfer_time;
  }

private:
//in seconds
  double _total_time;
  double _namelookup_time;
  double _connect_time;
  double _appconnect_time;
  double _pretransfer_time;
  double _starttransfer_time;



};

}






} //namespace node

#endif //WEBSITEMONITOR_NODE_CURLRESPONSE_H_
