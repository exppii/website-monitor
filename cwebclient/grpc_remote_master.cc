//
// Created by 刘优 on 16/5/12.
//

#include "cwebclient/grpc_remote_master.h"

#include <grpc++/grpc++.h>

#include "protos/master_service.grpc.pb.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

using ::grpc::Channel;

namespace webmonitor {

namespace webclient {

class GrpcMaster : public MasterInterface {

public:
  GrpcMaster (std::shared_ptr<Channel> channel)
      :_stub(MasterService::NewStub(channel)) {}
  ~GrpcMaster(){}

  ::grpc::Status create_job(const CreateJobRequest& req, CreateJobResponse* resp ) override{
    ::grpc::ClientContext ctx;
    _set_deadline(&ctx, _TIMEOUT);
    return _stub->CreateJob(&ctx, req, resp);
  }

  ::grpc::Status update_job(const UpdateJobRequest& req, UpdateJobResponse* resp ) override{
    ::grpc::ClientContext ctx;
    _set_deadline(&ctx, _TIMEOUT);
    return _stub->UpdateJob(&ctx, req, resp);
  }

  ::grpc::Status delete_job(const DeleteJobRequest& req, DeleteJobResponse* resp ) override{
    ::grpc::ClientContext ctx;
    _set_deadline(&ctx, _TIMEOUT);
    return _stub->DeleteJob(&ctx, req, resp);
  }

  ::grpc::Status list_job_status(const ListJobStatusRequest& req, ListJobStatusResponse* resp ) override{
    ::grpc::ClientContext ctx;
    _set_deadline(&ctx, _TIMEOUT);
    return _stub->ListJobStatus(&ctx, req, resp);
  }

  ::grpc::Status list_node_status(const ListNodeStatusRequest& req, ListNodeStatusResponse* resp ) override{
    ::grpc::ClientContext ctx;
    _set_deadline(&ctx, _TIMEOUT);
    return _stub->ListNodeStatus(&ctx, req, resp);
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

std::unique_ptr <MasterInterface> GrpcMasterPtr(
  std::shared_ptr<::grpc::Channel> channel) {
  return make_unique<GrpcMaster>(channel);
}

} //namespace webclient

} //namespace webmonitor
