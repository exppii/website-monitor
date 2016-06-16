//
// Created by 刘优 on 16/6/16.
//

#include "test/node_client_task_test.h"

std::unique_ptr<node::Options> NodeTaskTest::opt{nullptr};
std::shared_ptr<node::DataProcServiceInterface> NodeTaskTest::data_proc{nullptr};
std::vector<std::shared_ptr<node::TaskInterface> > NodeTaskTest::task_list{};

TEST_F(NodeTaskTest, HandleOptionsReturn) {
  EXPECT_EQ(5660, opt->get_taskserver_port());
  EXPECT_EQ(1000, opt->get_node_id());
  EXPECT_EQ("127.0.0.1", opt->get_taskserver_addr());

}