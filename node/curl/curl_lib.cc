//
// Created by 刘优 on 16/6/3.
//



#include "node/curl/curl_lib.h"

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

static size_t write_back(void *buffer, size_t size, size_t nmemb, void *userp)
{

  const char *d = static_cast<const char*>(buffer);

  std::string *b = static_cast<std::string*>(userp);

  size_t result = 0;

  if (b != nullptr) {
    b->append(d, size * nmemb);
    result = size * nmemb;
  }
  return result;

}



}


int get_html(const std::string &url, CurlResponse *resp);

int curl_get(const std::string &website, CurlResponse *resp) {

  return get_html(website, resp);
}

int curl_head(const std::string &url, CurlResponse *resp) {
  return 200;
}

int curl_post(const std::string &url, CurlResponse *resp) {
  return 200;
}


int get_html(const std::string &website, CurlResponse *resp) {
  CURL *curl;
  CURLcode curl_result;
  std::string data;
  curl = curl_easy_init();

  if (curl) {

    auto head_ptr = make_unique<curl_header>(
        std::initializer_list<std::string>{"Connection: keep-alive",
                                           "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
                                           "Content-Type: text/html",
                                           "Accept-Encoding: gzip, deflate",
                                           "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36"
        });

    //options
    curl_easy_setopt(curl, CURLOPT_URL, website.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head_ptr->get());
    //enable all supported built-in compressions
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
//    if (referer_url != nullptr) {
//      curl_easy_setopt(curl, CURLOPT_REFERER, referer_url);
//    }
//    curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);


    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                     &write_back); // our static function
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,
                     &data); // the string which will contain the html

    //ftime(&t1);//timestamp
    curl_result = curl_easy_perform(curl);
    //ftime(&t2);

    const char *url = nullptr;  //url to verify the connection worked
    long respcode; //response code of the http transaction
    long redirect; //number of redirects after we connect

    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &respcode);
    curl_easy_getinfo(curl, CURLINFO_REDIRECT_COUNT, &redirect);

    double time_in_seconds;

    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &time_in_seconds);
    resp->set_total_time(time_in_seconds);

    curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &time_in_seconds);
    resp->set_connect_time(time_in_seconds);

    curl_easy_getinfo(curl, CURLINFO_APPCONNECT_TIME, &time_in_seconds);
    resp->set_appconnect_time(time_in_seconds);
    curl_easy_getinfo(curl, CURLINFO_PRETRANSFER_TIME, &time_in_seconds);
    resp->set_pretransfer_time(time_in_seconds);
    curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &time_in_seconds);
    resp->set_starttransfer_time(time_in_seconds);

    resp->set_url(url);
    resp->set_respcode(respcode);
    resp->set_redirect(redirect);
    //resp->set_data(data);

    curl_easy_cleanup(curl); //cleanup curl information

  } else {
    //loger init curl meeting error
  }
  return 0;

}


}


} //namespace webmonitor