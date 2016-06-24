//
// Created by 刘优 on 16/6/16.
//

#include "test/node_client_task_test.h"

#include <thread>

std::unique_ptr<node::Options> NodeTaskTest::opt{nullptr};
std::shared_ptr<node::DataProcServiceInterface> NodeTaskTest::data_proc{nullptr};

static std::vector<std::shared_ptr<node::TaskInterface> > task_list;

static void _init_task_list() {

  node::TaskFactory factory;
  for (int i = 1; i < 8; i++) {
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

    task_list.push_back(factory.create(&task));
  }
}

TEST_F(NodeTaskTest, HandleOptionsReturn) {
  EXPECT_EQ(5660, opt->get_taskserver_port());
  EXPECT_EQ(1000, opt->get_node_id());
  EXPECT_EQ("127.0.0.1", opt->get_taskserver_addr());
  EXPECT_EQ("../cached/nodecache", opt->get_wal_path());

}

TEST_F(NodeTaskTest, HandleRunOneTask) {
  _init_task_list();
  for (auto& t : task_list) {
    t->run([](const nlohmann::json& data){
      return data_proc->add_data(data);
    });
  }


  std::this_thread::sleep_for(std::chrono::seconds(30));

}
