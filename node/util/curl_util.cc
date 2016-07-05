//
// Created by 刘优 on 16/6/3.
//



#include "node/util/curl_util.h"

#include <memory>
#include <fstream>
#include <string>
#include "node/curl/curl_header.h"
#include "node/response/curl_response.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

using curl::curl_header;




namespace webmonitor {



namespace node {

namespace {

static size_t write_back(void *buffer, size_t size, size_t nmemb, void *userp) {

  const char *d = static_cast<const char *>(buffer);

  std::string *b = static_cast<std::string *>(userp);

  size_t result = 0;

  if (b != nullptr) {
    b->append(d, size * nmemb);
    result = size * nmemb;
  }
  return result;

}

} //namespace


int get_html(const std::string &url, CurlResponse *resp);

int curl_perform(CURL *curl, CurlResponse *resp);

int curl_get(const std::string &website, CurlResponse *resp) {

  int ret = -1;
  CURL *curl = curl_easy_init();
  std::string data{};


  if (curl) {
    /* code */
    curl_easy_setopt(curl, CURLOPT_URL, website.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_back);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    //set_easy_opt to curl
    ret = curl_perform(curl, resp);
    if (ret == 0) resp->data = data;
    curl_easy_cleanup(curl); //cleanup curl information
  } else {
    //loger init curl meeting error
  }
  return ret;
}

int curl_head(const std::string &website, CurlResponse *resp) {

  int ret = -1;

  CURL *curl = curl_easy_init();
  std::string data{};


  if (curl) {
    /* code */
    curl_easy_setopt(curl, CURLOPT_URL, website.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_back);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    //set_easy_opt to curl
    ret = curl_perform(curl, resp);
    if (ret == 0) resp->data = data;
    curl_easy_cleanup(curl); //cleanup curl information
  } else {
    //loger init curl meeting error
  }
  return ret;
}

int curl_post(const std::string &url, CurlResponse *resp) {
  return -1;
}

int curl_perform(CURL *curl, CurlResponse *resp) {

  CURLcode curl_result;
  auto head_ptr = make_unique<curl_header>(
      std::initializer_list<std::string>{"Connection: keep-alive",
                                         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
                                         "Content-Type: text/html",
                                         "Accept-Encoding: gzip, deflate",
                                         "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36"
      });
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head_ptr->get());
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);

  //ftime(&t1);//timestamp
  curl_result = curl_easy_perform(curl);
  //ftime(&t2);
  resp->curl_code = static_cast<int>(curl_result);

  long respcode;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &respcode);
  resp->respcode = respcode;

  double time_in_seconds;

  curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &time_in_seconds);
  resp->total_time = time_in_seconds ;

  curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &time_in_seconds);
  resp->connect_time = time_in_seconds;

  curl_easy_getinfo(curl, CURLINFO_APPCONNECT_TIME, &time_in_seconds);
  resp->appconnect_time = time_in_seconds;
  curl_easy_getinfo(curl, CURLINFO_PRETRANSFER_TIME, &time_in_seconds);
  resp->pretransfer_time = time_in_seconds;
  curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &time_in_seconds);
  resp->starttransfer_time = time_in_seconds;

  return curl_result;

}

} //namespace node

} //namespace webmonitor
