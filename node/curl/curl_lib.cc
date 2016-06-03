//
// Created by 刘优 on 16/6/3.
//



#include "node/curl/curl_lib.h"

#include <memory>


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

  char *d = static_cast<char*>(buffer);

  std::string *b = static_cast<std::string*>(userp);

  size_t result = 0;
  if (b != NULL) {
    b->append(d, size * nmemb);
    result = size * nmemb;
  }
  return result;

}

}


int get_html(const std::string &url, CurlResponse *resp);

int curl_get(const std::string &url, CurlResponse *resp) {

  return get_html(url, resp);
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
  curl = curl_easy_init();

  if (curl) {

    auto head_ptr = make_unique<curl_header>(
        std::initializer_list<std::string>{"Connection: keep-alive",
                                           "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
                                           "Content-Type: text/html",
                                           "Accept-Encoding: gzip, deflate",
                                           "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/40.0.2214.111 Safari/537.36"
        });

    //options
    curl_easy_setopt(curl, CURLOPT_URL, website.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head_ptr->get());
    //enable all supported built-in compressions
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");

//    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
//    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connect_timeout);
//    if (referer_url != nullptr) {
//      curl_easy_setopt(curl, CURLOPT_REFERER, referer_url);
//    }
//    curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);


    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                     &write_back); // our static function
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,
                     resp); // the string which will contain the html

    //ftime(&t1);//timestamp
    curl_result = curl_easy_perform(curl);
    //ftime(&t2);

    char *url;  //url to verify the connection worked
    long respcode; //response code of the http transaction
    long redirect; //number of redirects after we connect
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL,
                      &url); //grabbing it from curl
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,
                      &respcode);// grabbing it from curl
    curl_easy_getinfo(curl, CURLINFO_REDIRECT_COUNT,
                      &redirect); //grabbing it from curl


    resp->set_url(url);
    resp->set_respcode(respcode);
    resp->set_redirect(redirect);

    curl_easy_cleanup(curl); //cleanup curl information

  } else {
    //loger init curl meeting error
  }


}


}


} //namespace webmonitor