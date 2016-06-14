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

  EXPECT_TRUE(cache->get_fresh_task_list(10,&taskmap2)) << "node: 10 get fresh task twice";
  EXPECT_EQ(0, taskmap2.size()) << "fresh task should be zero";
  EXPECT_EQ(10, cache->get_range_count(OLD))<< "old tasks did not change";
  EXPECT_EQ(25, cache->get_range_count(FRESH))<< "fresh tasks did not change";

  webmonitor::taskserver::LocalCachedUtilInterface::TaskMap taskmap3;

  EXPECT_TRUE(cache->get_fresh_task_list(30,&taskmap3)) << "node: 30 get fresh task";
  EXPECT_EQ(10, taskmap3.size()) << "node: 30 should have 10 tasks";
  EXPECT_EQ(20, cache->get_range_count(OLD))<< "node: 30 would put 10 tasks to regular";
  EXPECT_EQ(15, cache->get_range_count(FRESH))<< "rest fresh task should be 35 - 20";

  webmonitor::taskserver::LocalCachedUtilInterface::TaskMap taskmap4;

  EXPECT_TRUE(cache->get_fresh_task_list(40,&taskmap4))<< "node: 40 get fresh task";
  EXPECT_EQ(5, taskmap4.size())<< "node: 40 should have 5 tasks";
  EXPECT_EQ(25, cache->get_range_count(OLD))<< "node: 40 would put 5 tasks to regular";
  EXPECT_EQ(10, cache->get_range_count(FRESH)) << "rest fresh task should be 35 - 20";

}

TEST_F(LocalCacheTest, HandleUpdateJob) {

  JobDef job;
  job.set_id(3000);
  auto task = job.mutable_task();

  task->set_frequency(10);
  task->set_dest("qq.com");
  task->set_type(webmonitor::TaskDef::DNS);
  //remove node 20 and add node 40 & 50
  job.add_node_list(10);
  job.add_node_list(40);
  job.add_node_list(30);
  job.add_node_list(50);

  EXPECT_TRUE(cache->update_job(job)) << "update job to cache: " << job.id();

  EXPECT_EQ(10, cache->get_range_count(JOBKEY)) << "should have 10 jobs";

  EXPECT_EQ(9, cache->get_range_count(OLD + _id_to_string(10))) << "node10 should have 9 old jobs";
  EXPECT_EQ(1, cache->get_range_count(FRESH + _id_to_string(10))) << "node10 should have 1 new jobs";

  EXPECT_EQ(0, cache->get_range_count(OLD + _id_to_string(20))) << "node20 should have 0 old jobs";
  EXPECT_EQ(9, cache->get_range_count(FRESH + _id_to_string(20))) << "node20 should have 9 new jobs";

  EXPECT_EQ(9, cache->get_range_count(OLD + _id_to_string(30))) << "node30 should have 0 old jobs";
  EXPECT_EQ(1, cache->get_range_count(FRESH + _id_to_string(30))) << "node30 should have 10 new jobs";

  EXPECT_EQ(5, cache->get_range_count(OLD + _id_to_string(40))) << "node40 should have 5 old jobs";
  EXPECT_EQ(1, cache->get_range_count(FRESH + _id_to_string(40))) << "node40 should have 1 new jobs";

  EXPECT_EQ(0, cache->get_range_count(OLD + _id_to_string(50))) << "node50 should have 0 fresh jobs";
  EXPECT_EQ(1, cache->get_range_count(FRESH + _id_to_string(50))) << "node50 should have 1 fresh jobs";

  EXPECT_EQ(23, cache->get_range_count(OLD)) << "del node10,30 so: 15 -2 = 14";
  EXPECT_EQ(13, cache->get_range_count(FRESH))
  << "add node10,30,40,50 fresh and del node20 so: 10 + 4 -1 = 13";


}

TEST_F(LocalCacheTest, HandleGetTheWholeTask) {

  webmonitor::taskserver::LocalCachedUtilInterface::TaskMap taskmap;

  EXPECT_TRUE(cache->get_fresh_task_list(40,&taskmap));
  EXPECT_EQ(1, taskmap.size());
  EXPECT_EQ(24, cache->get_range_count(OLD)); //10 + 5
  EXPECT_EQ(12, cache->get_range_count(FRESH)); //35 -10 - 5

  webmonitor::taskserver::LocalCachedUtilInterface::TaskMap taskmap2;
  EXPECT_TRUE(cache->get_whole_task_list(10,&taskmap2));
  EXPECT_EQ(10, taskmap2.size());

  EXPECT_EQ(10, cache->get_range_count(OLD + _id_to_string(10))) << "node10 should have 9 old jobs";
  EXPECT_EQ(0, cache->get_range_count(FRESH + _id_to_string(10))) << "node10 should have 1 new jobs";

  EXPECT_EQ(0, cache->get_range_count(OLD + _id_to_string(20))) << "node20 should have 0 old jobs";
  EXPECT_EQ(9, cache->get_range_count(FRESH + _id_to_string(20))) << "node20 should have 9 new jobs";

  EXPECT_EQ(9, cache->get_range_count(OLD + _id_to_string(30))) << "node30 should have 0 old jobs";
  EXPECT_EQ(1, cache->get_range_count(FRESH + _id_to_string(30))) << "node30 should have 10 new jobs";

  EXPECT_EQ(6, cache->get_range_count(OLD + _id_to_string(40))) << "node40 should have 5 old jobs";
  EXPECT_EQ(0, cache->get_range_count(FRESH + _id_to_string(40))) << "node40 should have 1 new jobs";

  EXPECT_EQ(0, cache->get_range_count(OLD + _id_to_string(50))) << "node50 should have 0 fresh jobs";
  EXPECT_EQ(1, cache->get_range_count(FRESH + _id_to_string(50))) << "node50 should have 1 fresh jobs";


}


TEST_F(LocalCacheTest, HandleDeleteTask) {
  for (size_t i = 1; i < 4; i++) {
    EXPECT_TRUE(cache->del_job(i * 1000)) << "delete job from cache: " << i * 1000;
  }

  EXPECT_EQ(7, cache->get_range_count(JOBKEY)) << "should have 7 jobs";

  EXPECT_EQ(7, cache->get_range_count(OLD + _id_to_string(10))) << "node10 should have 9 old jobs";
  EXPECT_EQ(0, cache->get_range_count(FRESH + _id_to_string(10))) << "node10 should have 1 new jobs";

  EXPECT_EQ(0, cache->get_range_count(OLD + _id_to_string(20))) << "node20 should have 0 old jobs";
  EXPECT_EQ(7, cache->get_range_count(FRESH + _id_to_string(20))) << "node20 should have 9 new jobs";

  EXPECT_EQ(7, cache->get_range_count(OLD + _id_to_string(30))) << "node30 should have 0 old jobs";
  EXPECT_EQ(0, cache->get_range_count(FRESH + _id_to_string(30))) << "node30 should have 10 new jobs";

  EXPECT_EQ(4, cache->get_range_count(OLD + _id_to_string(40))) << "node40 should have 5 old jobs";
  EXPECT_EQ(0, cache->get_range_count(FRESH + _id_to_string(40))) << "node40 should have 1 new jobs";

  EXPECT_EQ(0, cache->get_range_count(OLD + _id_to_string(50))) << "node50 should have 0 fresh jobs";
  EXPECT_EQ(0, cache->get_range_count(FRESH + _id_to_string(50))) << "node50 should have 1 fresh jobs";


}
