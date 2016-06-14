//
// Created by 刘优 on 16/5/11.
//

#ifndef WEBSITEMONITOR_WEBCLIENT_MASTER_INTERFACE_H_
#define WEBSITEMONITOR_WEBCLIENT_MASTER_INTERFACE_H_

#include <grpc++/support/status.h>
#include "protos/master_service.pb.h"

namespace webmonitor {

namespace webclient {

class MasterInterface {

public:
  virtual ~MasterInterface() {}

  virtual ::grpc::Status create_job(const CreateJobRequest&, CreateJobResponse* ) = 0;
  virtual ::grpc::Status update_job(const UpdateJobRequest&, UpdateJobResponse* ) = 0;
  virtual ::grpc::Status delete_job(const DeleteJobRequest&, DeleteJobResponse* ) = 0;
  virtual ::grpc::Status list_job_status(const ListJobStatusRequest&, ListJobStatusResponse* ) = 0;
  virtual ::grpc::Status list_node_status(const ListNodeStatusRequest&, ListNodeStatusResponse* ) = 0;

};

}



} //namespace webmonitor

#endif //WEBSITEMONITOR_WEBCLIENT_MASTER_INTERFACE_H_
