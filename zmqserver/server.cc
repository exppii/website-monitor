//
// Created by 刘优 on 16/6/20.
//
#include "cppzmq/zmq.hpp"
#include <string>
#include <iostream>
using namespace std;
//using namespace webmonitor;


int main(int argc, char *argv[]) {

#ifdef __linux
  cout << "posix" << endl;
#endif

  std::string connectstr("tcp://*:8899");
  zmq::context_t context(1);
  zmq::socket_t receiver(context, ZMQ_PULL);
  receiver.bind(connectstr.c_str());

  while (true) {
    zmq::message_t message;

    std::cout << "Receiving..." << std::endl;
    receiver.recv(&message);
    //std::string s(static_cast<char*>(message.data(),message.size()));
    std::string rpl = std::string(static_cast<char *>(message.data()), message.size());
    std::cout << rpl << "data size: " << rpl.size() << std::endl;
#ifdef NDEBUG
    std::string after = decompress(rpl);
    cout << "after : " << after << " size: " << after.size() << endl;
#endif
    if (rpl == "END") {
      break;
    }
  }


}