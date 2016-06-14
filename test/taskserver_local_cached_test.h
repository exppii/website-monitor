#ifndef WEBSITEMONITOR_TEST_TASKSERVER_LOCAL_CACHED_TEST_H_
#define  WEBSITEMONITOR_TEST_TASKSERVER_LOCAL_CACHED_TEST_H_

#include <vector>
#include <stdlib.h>

#include "common/utils.h"
#include "common/varint.h"
#include "taskserver/options.h"
#include "taskserver/local_cache_util.h"
#include "taskserver/local_cache_util_interface.h"
#include "protos/master_service.pb.h"
#include "protos/task_content.pb.h"
#include "gtest/gtest.h"



using namespace webmonitor;


class LocalCacheTest : public ::testing::Test {

protected:
  static void SetUpTestCase() {
    opt.reset(new taskserver::Options("../conf/taskserver.config"));
    cache = webmonitor::taskserver::LevelDBCachedUtilUniquePtr(opt.get());
    _init_job_list();
  }

  static void TearDownTestCase() {
    const std::string command = "rm -rf " + opt->get_cachefile_path() + "cacheddb";
    system(command.c_str());
  }


  static void _init_job_list() {

    for (int i = 1; i < 11; i++) {
      JobDef job;
      job.set_id(i * 1000);
      auto task = job.mutable_task();

      task->set_frequency(i);
      task->set_dest("baidu.com");
      task->set_type(webmonitor::TaskDef::HTTP);

      job.add_node_list(10);
      job.add_node_list(20);
      job.add_node_list(30);
      if (i % 2 == 0) {
        job.add_node_list(40);
      }
      job_list.push_back(job);
    }
  }

  std::string _id_to_string(const uint64_t& id) {
    char buf[8] = {0};
    encode_fixint64(buf, id);
    return std::string(buf,8);
  }

  static std::unique_ptr<taskserver::Options> opt;
  static std::unique_ptr<taskserver::LocalCachedUtilInterface> cache;
  static std::vector<JobDef> job_list;

};

#endif //WEBSITEMONITOR_TEST_TASKSERVER_LOCAL_CACHED_TEST_H_
