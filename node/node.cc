//
// Created by 刘优 on 16/6/3.
//

#include <iostream>
#include <grpc++/grpc++.h>
#include "node/grpc_remote_node.h"


using namespace std;
using namespace webmonitor;

int main(int argc, char const *argv[]) {

  auto channel = grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials());

  auto client = GrpcNodePtr(channel);

  GetJobRequest req;
  GetJobResponse resp;

  auto info = req.mutable_node();

  info->set_id(2000);

  auto status = client->get_job(&req,&resp);

  if(status.ok()) {
    for (const auto& t : resp.task_list()) {
      cout << t.id() << endl;
    }
  }


}