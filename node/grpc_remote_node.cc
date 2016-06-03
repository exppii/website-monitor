//
// Created by 刘优 on 16/5/16.
//
#include "node/grpc_remote_node.h"

#include <grpc++/grpc++.h>

#include "protos/master_service.grpc.pb.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

using ::grpc::Channel;

namespace webmonitor {

class GrpcNode : public NodeInterface {

public:
  explicit GrpcNode (std::shared_ptr<Channel> channel)
      :_stub(MasterService::NewStub(channel)) {}
  ~GrpcNode () {}

  grpc::Status get_job(const GetJobRequest* req, GetJobResponse* resp) override {
    ::grpc::ClientContext ctx;
    _set_deadline(&ctx, _TIMEOUT);
    return _stub->GetJob(&ctx, *req, resp);
  }

  grpc::Status report_status(const ReportStatusRequest* req, ReportStatusResponse* resp) override {
    ::grpc::ClientContext ctx;
    _set_deadline(&ctx, _TIMEOUT);
    return _stub->ReportStatus(&ctx, *req, resp);
  }

private:

  void _set_deadline(::grpc::ClientContext* ctx, int64_t time_in_ms) {
    if(time_in_ms > 0) {
      ctx->set_deadline(gpr_time_from_millis(time_in_ms,GPR_TIMESPAN));
    }
  }

private:
  /* data */
  const int64_t _TIMEOUT = 3*1000*10;
  std::unique_ptr<MasterService::Stub> _stub;
};

std::unique_ptr<NodeInterface> GrpcNodePtr(std::shared_ptr<::grpc::Channel>
channel){
  return make_unique<GrpcNode>(channel);
}

} //namespace elon



