//
// Created by 刘优 on 16/6/16.
//

#ifndef WEBSITEMONITOR_TEST_NODE_CLIENT_TASK_TEST_H
#define WEBSITEMONITOR_TEST_NODE_CLIENT_TASK_TEST_H

#include <vector>
#include <stdlib.h>

#include "common/utils.h"
//#include "common/varint.h"
#include "node/options.h"
#include "node/task/task_factory.h"
#include "node/data_proc_service.h"
#include "protos/master_service.pb.h"
#include "protos/task_content.pb.h"
#include "node/logger.h"
#include "gtest/gtest.h"

using namespace webmonitor;

class NodeTaskTest : public ::testing::Test {

protected:
  static void SetUpTestCase() {
    opt.reset(new node::Options("../conf/node_service.config"));

    auto outlog = spdlog::stdout_logger_mt(node::NODE_TAG);
    outlog->set_level(spdlog::level::debug);

    data_proc.reset(node::NewDataProcServicePtr(opt.get()));
    data_proc->start();
    _init_task_list();
  }

  static void TearDownTestCase() {
    data_proc->stop();
  }


  static void _init_task_list() {

    node::TaskFactory factory;
    for (int i = 1; i < 11; i++) {
      TaskDef task;


      task.set_frequency(i* 5);
      task.set_dest("baidu.com");
      task.set_type(webmonitor::TaskDef::HTTP);

      task.set_status(TaskDef::RUN);
      task.set_type(TaskDef::HTTP);

      HTTP_CONTENT content;
      content.set_match_cmd(HTTP_CONTENT::NOT_CARE);
      content.set_method(HTTP_CONTENT::GET);
      content.set_match_content("404");

      task.mutable_content()->PackFrom(content);

      ;

      task_list.push_back(factory.create(&task,data_proc));
    }
  }


  static std::unique_ptr<node::Options> opt;
  static std::shared_ptr<node::DataProcServiceInterface> data_proc;
  static std::vector<std::shared_ptr<node::TaskInterface> > task_list;

};


#endif //WEBSITEMONITOR_TEST_NODE_CLIENT_TASK_TEST_H
