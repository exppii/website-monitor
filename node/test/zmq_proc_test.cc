//
// Created by 刘优 on 16/7/5.
//
#include <iostream>

#include "node/dataproc/zmq_process.h"

#include "node/dataproc/data_proc_interface.h"
#include <memory>

using namespace webmonitor;
using namespace std;

int main(int argc, char const *argv[]) {

  std::string ip = "127.0.0.1";
  unsigned int port = 8899;

  std::unique_ptr<node::DataProcInterface> proc{node::NewZMQProcPtr(ip,port)};

  string data = "test pre test pre test pre test pre";

  do {
    string s;
    cout << "==> ";
    getline(cin, s);

    if (s[0] == '/') break;
    auto msg = data + s;

    cout << "result: " << proc->proc(&msg) << endl;

  } while (cin.good());


  return 0;



}