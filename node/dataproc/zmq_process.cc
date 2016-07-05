//
// Created by 刘优 on 16/6/5.
//

#include "node/dataproc/zmq_process.h"

#include "cppzmq/zmq.hpp"

#include "node/dataproc/data_proc_interface.h"

namespace webmonitor {

namespace node {

class ZMQProc : public DataProcInterface {
public:

 //@see http://api.zeromq.org/3-1:zmq-setsockopt
  ZMQProc(const std::string& url, const uint32_t port)
      :_context(1), _socket(_context, ZMQ_PUSH) { //context with one io_threads
    _socket.setsockopt(ZMQ_SNDHWM, 1); //socket queue size
   //Pending messages shall be discarded immediately when the socket is
   //closed with zmq_close()
    _socket.setsockopt(ZMQ_LINGER, 0);
    auto addr = "tcp://" + url + ":" + std::to_string(port);
    _socket.connect(addr);

  }

  ~ZMQProc() { }

  bool proc(std::string* ) override;

  std::string proc_name() const override { return std::string("zmq proc");}

private:

  zmq::context_t _context;
  zmq::socket_t _socket;

};

//TODO
bool ZMQProc::proc(std::string* data) {
  zmq::message_t msg(data->cbegin(),data->cend());
  return _socket.send(msg,ZMQ_NOBLOCK) == 0;

}

DataProcInterface* NewZMQProcPtr(const std::string& url, const uint32_t port){
  return new ZMQProc(url, port);
}

} //namespace node
} //namespace webmonitor
