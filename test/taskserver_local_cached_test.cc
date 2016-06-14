//
// Created by 刘优 on 16/6/13.
//


//#include <leveldb/slice.h>

#include "taskserver_local_cached_test.h"


std::unique_ptr<taskserver::Options> LocalCacheTest::opt{nullptr};
std::unique_ptr<taskserver::LocalCachedUtilInterface> LocalCacheTest::cache{
    nullptr};

std::vector<JobDef> LocalCacheTest::job_list{};


const std::string JOBKEY = "j"; //job jobcontent kv pre
const std::string FRESH = "f"; //fresh node job relationship pre
const std::string OLD = "o"; //old node job relationship pre
const std::string RALATED = "r"; //job node relationship pre relate

TEST(file_existsTest, HandleTrueReturn) {
  EXPECT_TRUE(file_exists("../conf/taskserver.config"));
  EXPECT_TRUE(file_exists("../conf/taskserver_log.config"));

//  char buf[LONG_SIZE] = {0};
//  uint64_t i = 0,j;
//  webmonitor::encode_fixint64(buf, id);
//  return std::string(buf,LONG_SIZE);
//  leveldb::Slice a("j" + )

}

TEST_F(LocalCacheTest, HandleOptionsReturn) {
  EXPECT_EQ(5660, opt->get_listen_port());
  EXPECT_EQ(50, opt->get_cache_size());
  EXPECT_EQ("0.0.0.0", opt->get_listen_addr());
  EXPECT_EQ("../cached/", opt->get_cachefile_path());
}

TEST_F(LocalCacheTest, HandleStoreJob) {

  for (const auto& job : job_list) {
    EXPECT_TRUE(cache->store_job(job)) << "store job to cache: " << job.id();
  }

  EXPECT_EQ(35, cache->get_range_count(RALATED)) << "should have 10*3 + 5 job +node relations";
  EXPECT_EQ(35, cache->get_range_count(FRESH)) << "should have 10*3 + 5 reserved node +job relations";
  EXPECT_EQ(10, cache->get_range_count(JOBKEY)) << "should have 10 jobs"; //10 jobs

  for (size_t i = 1; i < 4; i++) {
    EXPECT_EQ(10, cache->get_range_count(FRESH + _id_to_string(i*10))) <<
    "node : " << i* 10 << "should have 10 reserved jobs";
  }
  EXPECT_EQ(5, cache->get_range_count(FRESH + _id_to_string(40))) <<
  "node : 40 should have 5 reserved jobs";

}

TEST_F(LocalCacheTest, HandleGetFreshTask) {

  webmonitor::taskserver::LocalCachedUtilInterface::TaskMap taskmap;

  EXPECT_TRUE(cache->get_fresh_task_list(10,&taskmap)) << "node: 10 get fresh task";

  EXPECT_EQ(10, taskmap.size())<< "node: 10 should have 10 tasks";
  EXPECT_EQ(10, cache->get_range_count(OLD)) << "node: 10 would put 10 tasks to regular";
  EXPECT_EQ(25, cache->get_range_count(FRESH)) << "rest fresh task should be 35 - 10";

  webmonitor::taskserver::LocalCachedUtilInterface::TaskMap taskmap2;

  EXPECT_TRUE(cache->get_fresh_task_list(10,&taskmap2));
  EXPECT_EQ(0, taskmap2.size());
  EXPECT_EQ(10, cache->get_range_count(OLD)); //10*3 + 5*
  EXPECT_EQ(25, cache->get_range_count(FRESH)); //10*3 + 5*

  webmonitor::taskserver::LocalCachedUtilInterface::TaskMap taskmap3;

  EXPECT_TRUE(cache->get_fresh_task_list(40,&taskmap3));
  EXPECT_EQ(5, taskmap3.size());
  EXPECT_EQ(15, cache->get_range_count(OLD)); //10 + 5
  EXPECT_EQ(20, cache->get_range_count(FRESH)); //35 -10 - 5
}

TEST_F(LocalCacheTest, HandleUpdateJob) {
  //before update
  //job count 10
  //re

  // JobDef job;
  // job.set_id(3000);
  // auto task = job.mutable_task();
  //
  // task->set_frequency(10);
  // task->set_dest("qq.com");
  // task->set_type(webmonitor::TaskDef::DNS);
  // //remove node 10 and add node 40
  // job.add_node_list(40);
  // job.add_node_list(20);
  // job.add_node_list(30);
  //
  // EXPECT_TRUE(cache->update_job(job)) << "update job to cache: " << job.id();
  //
  // EXPECT_EQ(15, cache->get_range_count("r")); //10 + 5
  // EXPECT_EQ(20, cache->get_range_count("s")); //35 -10 - 5

}

TEST_F(LocalCacheTest, HandleGetTheWholeTask) {
  // JobDef job;
  // job.set_id(11000);
  // auto task = job.mutable_task();
  //
  // task->set_frequency(i);
  // task->set_dest("baidu.com");
  // task->set_type(webmonitor::TaskDef::DNS);
  //
  // job.add_node_list(10);
  // job.add_node_list(20);
  // job.add_node_list(30);
  // EXPECT_TRUE(cache->store_job(job)) << "store job to cache: " << job.id();


//  webmonitor::taskserver::LocalCachedUtilInterface::TaskMap taskmap;
//
//  EXPECT_TRUE(cache->(10,&taskmap));
//  cache->print_all();
//  EXPECT_EQ(10, cache->get_range_count("r")); //10*3 + 5*
//  EXPECT_EQ(25, cache->get_range_count("s")); //10*3 + 5*

}
