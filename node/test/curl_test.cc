//
// Created by 刘优 on 16/6/3.
//

#include <iostream>
#include <string>
#include "node/curl/curl_lib.h"
#include "node/response/curl_response.h"
#include "common/utils.h"
#include <fstream>

using namespace std;
using namespace webmonitor::node;

void fetch_url_test() {

  ifstream file("../data/url_data.txt");

  if(file.is_open()) {
    std::string website;
    while (std::getline(file,website)) {
      cout << "current url: " << website << endl;
      CurlResponse resp;

      curl_head(website, &resp);
      cout << resp.dump() << endl;

      sleep(1);
    }
    file.close();
  }


}

int main(int argc, char const *argv[]) {

  fetch_url_test();

}
