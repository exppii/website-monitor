//
// Created by 刘优 on 16/6/3.
//

#ifndef WEBSITEMONITOR_NODE_CURL_LIB_H_
#define WEBSITEMONITOR_NODE_CURL_LIB_H_

#include <string>


namespace webmonitor {

namespace node {

class CurlResponse;

int curl_get(const std::string& url, CurlResponse* resp);

int curl_head(const std::string& url, CurlResponse* resp);

int curl_post(const std::string& url, CurlResponse* resp);

}




} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_CURL_LIB_H_
