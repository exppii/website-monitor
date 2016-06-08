//
// Created by 刘优 on 16/5/12.
//

#include "taskserver/grpc_master_service.h"

#include <mutex>
#include <grpc++/server_builder.h>

#include "taskserver/master.h"
#include "taskserver/logger.h"
#include "common/rpc/grpc_call.h"


#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

#include "protos/master_service.grpc.pb.h"

namespace webmonitor {

namespace taskserver {

class GrpcMasterService : public AsyncServiceInterface {
public:
  GrpcMasterService(::grpc::ServerBuilder* builder) {
    _logger->info("init grpc master service...");
    builder->RegisterService(&_service);
    _logger->info("add completion queue .");
    _cq = builder->AddCompletionQueue().release();
  }

  ~GrpcMasterService() {
    //  delete _shutdown_alarm;
    delete _cq;
    delete _master_impl;
    _logger->info("grpc service has been shutdowned.");
  }

#define ENQUEUE_REQUEST(method, supports_cancel)                              \
  do {                                                                        \
    std::unique_lock<std::mutex> l(_mtx);                                     \
    if (!_is_shutdown) {                                                      \
      Call<GrpcMasterService, MasterService::AsyncService,                    \
           method##Request, method##Response>::                               \
          EnqueueRequest(&_service, _cq,                                      \
                         &MasterService::AsyncService::Request##method,       \
                         &GrpcMasterService::method##Handler,                 \
                         (supports_cancel));                                  \
    }                                                                         \
  } while (0)

  //TODO using multi thread and async
  void handle_grpc_loop() override {
    _logger->info("push all resquse handle...");
    ENQUEUE_REQUEST(CreateJob, false);
    ENQUEUE_REQUEST(UpdateJob,true);
    ENQUEUE_REQUEST(DeleteJob,true);
    ENQUEUE_REQUEST(GetJob,true);
    ENQUEUE_REQUEST(ListJobStatus,true);
    ENQUEUE_REQUEST(ListNodeStatus,true);
    ENQUEUE_REQUEST(ReportStatus, true);
    void* tag;
    bool ok;

    while (_cq->Next(&tag,&ok)) {
      UntypedCall<GrpcMasterService>::Tag* callback_tag =
          static_cast<UntypedCall<GrpcMasterService>::Tag*>(tag);
      if (callback_tag) {
        callback_tag->OnCompleted(this, ok);
        delete callback_tag;
      } else {
        // NOTE(mrry): A null `callback_tag` indicates that this is
        // the shutdown alarm.
        _cq->Shutdown();
      }
    }

  }

  void shutdown() override {
    bool did_shutdown = false;
    {
      std::unique_lock<std::mutex> l(_mtx);
      if (!_is_shutdown) {
        _is_shutdown = true;
        did_shutdown = true;
      }
    }
    if(did_shutdown) {
//      _shutdown_alarm = new ::grpc::Alarm(_cq,gpr_now(GPR_CLOCK_MONOTONIC),
//                                          nullptr);
    }
  }


private:

  std::shared_ptr<spdlog::logger> _logger{spdlog::get(SERVER_TAG)};

  template <class RequestMessage, class ResponseMessage>
  using MasterCall = Call<GrpcMasterService, MasterService::AsyncService,
      RequestMessage, ResponseMessage>;

  bool _is_shutdown{false};
  MasterService::AsyncService _service;
  ::grpc::ServerCompletionQueue* _cq;  // Owned.
  Master* _master_impl;                // Owned.
  // ::grpc::Alarm* _shutdown_alarm;

  std::mutex _mtx;



private:

  void CreateJobHandler(MasterCall<CreateJobRequest, CreateJobResponse>* call) {
    _master_impl->create_job(&call->request, &call->response,
                             [call](const ::grpc::Status& status) {
                               call->SendResponse(status);
                             });
    ENQUEUE_REQUEST(CreateJob, true);
  }

  void UpdateJobHandler(MasterCall<UpdateJobRequest, UpdateJobResponse>* call) {
    _master_impl->update_job(&call->request, &call->response,
                             [call](const ::grpc::Status& status) {
                               call->SendResponse(status);
                             });
    ENQUEUE_REQUEST(CreateJob, true);
  }

  void DeleteJobHandler(MasterCall<DeleteJobRequest, DeleteJobResponse>* call) {
    _master_impl->delete_job(&call->request, &call->response,
                             [call](const ::grpc::Status& status) {
                               call->SendResponse(status);
                             });
    ENQUEUE_REQUEST(CreateJob, true);
  }

  void ListJobStatusHandler(MasterCall<ListJobStatusRequest, ListJobStatusResponse>* call) {
    _master_impl->list_job_status(&call->request, &call->response,
                                  [call](const ::grpc::Status& status) {
                                    call->SendResponse(status);
                                  });
    ENQUEUE_REQUEST(CreateJob, true);
  }

  void ListNodeStatusHandler(MasterCall<ListNodeStatusRequest, ListNodeStatusResponse>* call) {
    _master_impl->list_node_status(&call->request, &call->response,
                                   [call](const ::grpc::Status& status) {
                                     call->SendResponse(status);
                                   });
    ENQUEUE_REQUEST(CreateJob, true);
  }

  void GetJobHandler(MasterCall<GetJobRequest, GetJobResponse>* call) {
    _master_impl->get_job(&call->request, &call->response,
                          [call](const ::grpc::Status& status) {
                            call->SendResponse(status);
                          });
    ENQUEUE_REQUEST(CreateJob, true);
  }

  void ReportStatusHandler(MasterCall<ReportStatusRequest, ReportStatusResponse>* call) {
    _master_impl->report_status(&call->request, &call->response,
                                [call](const ::grpc::Status& status) {
                                  call->SendResponse(status);
                                });
    ENQUEUE_REQUEST(CreateJob, true);
  }

};

std::unique_ptr<AsyncServiceInterface> GrpcMasterServicePtr(::grpc::ServerBuilder
                                                            * builder){
  return make_unique<GrpcMasterService>(builder);
}


} //namespace taskserver

} //namespace elon


