//
// Created by 刘优 on 16/5/16.
//
#include "node/grpc_remote_node.h"

#include <grpc++/grpc++.h>

#include "node/node_interface.h"
#include "protos/master_service.grpc.pb.h"


using ::grpc::Channel;

namespace webmonitor {

namespace node {

class GrpcNode : public NodeInterface {

public:
  explicit GrpcNode(std::shared_ptr<Channel> channel)
      : _stub(MasterService::NewStub(channel)) { }

  ~GrpcNode() { }

  grpc::Status get_job(const GetJobRequest &req,
                       GetJobResponse *resp) override {
    ::grpc::ClientContext ctx;
    _set_deadline(&ctx, _TIMEOUT);
    return _stub->GetJob(&ctx, req, resp);
  }

  grpc::Status report_status(const ReportStatusRequest &req,
                             ReportStatusResponse *resp) override {
    ::grpc::ClientContext ctx;
    _set_deadline(&ctx, _TIMEOUT);
    return _stub->ReportStatus(&ctx, req, resp);
  }

private:

  void _set_deadline(::grpc::ClientContext *ctx, int64_t time_in_ms) {
    if (time_in_ms > 0) {
      ctx->set_deadline(gpr_time_from_millis(time_in_ms, GPR_TIMESPAN));
    }
  }

private:
  /* data */
  const int64_t _TIMEOUT = 3 * 1000 * 10;
  std::unique_ptr<MasterService::Stub> _stub;
};

NodeInterface* NewGrpcNodePtr(std::shared_ptr<::grpc::Channel> channel) {
  return new GrpcNode(channel);
}

}


} //namespace elon



