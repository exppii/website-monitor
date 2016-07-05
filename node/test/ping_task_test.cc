//
// Created by 刘优 on 16/7/5.
//
#include <iostream>

#include "node/response/ping_response.h"

#include "node/util/ping_util.h"

using namespace webmonitor;
using namespace std;

int main(int argc, char const *argv[]) {

  std::string ip = "119.29.29.29";

  node::PingResponse resp;

  if(node::ping_ip(ip,&resp)) {
    cout << resp.to_json() << endl;
  } else {
    cout << "ping " << ip << " meet errors ." << endl;
  }
  return 0;



}