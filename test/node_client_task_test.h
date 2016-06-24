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
  }

  static void TearDownTestCase() {
    data_proc->stop();
    const std::string command = "rm -rf " + opt->get_wal_path();
    system(command.c_str());
  }

  static std::unique_ptr<node::Options> opt;
  static std::shared_ptr<node::DataProcServiceInterface> data_proc;

};


#endif //WEBSITEMONITOR_TEST_NODE_CLIENT_TASK_TEST_H
