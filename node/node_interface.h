//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR_NODE_NODE_INTERFACE_H_
#define WEBSITEMONITOR_NODE_NODE_INTERFACE_H_

#include <grpc++/support/status.h>
#include "protos/master_service.pb.h"

namespace webmonitor {

namespace node {

class NodeInterface {

public:
  virtual ~NodeInterface() {}

  virtual ::grpc::Status get_job(const GetJobRequest*, GetJobResponse* ) = 0;

  virtual ::grpc::Status report_status(const ReportStatusRequest*, ReportStatusResponse* ) = 0;

};

}



} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_NODE_INTERFACE_H_
