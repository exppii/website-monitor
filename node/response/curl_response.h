//
// Created by 刘优 on 16/6/3.
//

#ifndef WEBSITEMONITOR_NODE_CURLRESPONSE_H_
#define WEBSITEMONITOR_NODE_CURLRESPONSE_H_

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

  void set_url(std::string url) {
    _url = url;
  }

private:
  std::string _url;

  int _respcode{404};
  int _redirect;

};

}






} //namespace node

#endif //WEBSITEMONITOR_NODE_CURLRESPONSE_H_
