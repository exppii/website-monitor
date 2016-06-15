#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <grpc++/grpc++.h>

#include "grpc_remote_master.h"

#include "protos/master_service.grpc.pb.h"
#include "protos/task_content.pb.h"

using namespace std;
using namespace webmonitor;

void init_job_list(std::vector<JobDef> &job_list) {

  for (int i = 1; i < 3; i++) {
    JobDef job;
    job.set_id(i * 1000);
    auto task = job.mutable_task();

    task->set_frequency(10);
    task->set_dest("baidu.com");
    task->set_type(webmonitor::TaskDef::HTTP);
    task->set_status(webmonitor::TaskDef::RUN);

    job.add_node_list(1000);
    job.add_node_list(20);
    job.add_node_list(30);
    if (i % 2 == 0) {
      job.add_node_list(40);
      task->set_status(webmonitor::TaskDef::STOP);
    }
    job_list.push_back(job);
  }
}

int main(int argc, char const *argv[]) {
  cout << "hello web client" << endl;

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  const std::string url{"127.0.0.1:5660"};

  auto channel = grpc::CreateChannel(url, grpc::InsecureChannelCredentials());

  auto grpc_master = webclient::GrpcMasterPtr(channel);

  std::vector<JobDef> job_list;

  init_job_list(job_list);

  {
    CreateJobRequest req;
    CreateJobResponse resp;
    for (const auto &job: job_list) {

      req.mutable_job()->CopyFrom(job);

      std::cout << "now create one job : " << job.id() << std::endl;
      auto status = grpc_master->create_job(req, &resp);

      if (!status.ok()) {
        std::cout << "create job " << job.id() << "failed. error code: " <<
        status.error_code() << " error message: " << status.error_message();
      } else {
        std::cout << "job: " << job.id() << " ret message: " <<
        resp.response().message() << std::endl;
      }


      std::this_thread::sleep_for(std::chrono::seconds(30));

    }
  }
  //std::this_thread::sleep_for(std::chrono::seconds(30));
  {
    DeleteJobRequest req;
    DeleteJobResponse resp;
    std::cout << "now dele one job : " << 1000 << std::endl;

    req.set_job_id(1000);

    auto status = grpc_master->delete_job(req, &resp);

    if (!status.ok()) {
      std::cout << "del job " << 1000 << "failed. error code: " <<
      status.error_code() << " error message: " << status.error_message();
    } else {
      std::cout << "job: " << 1000 << " ret message: " <<
      resp.response().message() << std::endl;
    }

  }





  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end - start;
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);

  std::cout << "finished computation at " << std::ctime(&end_time)
  << "elapsed time: " << elapsed_seconds.count() << "s\n";
  return 0;
}
