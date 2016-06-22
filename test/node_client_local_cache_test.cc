//
// Created by 刘优 on 16/6/23.
//

#include "test/node_client_local_cache_test.h"

#include <iostream>

std::unique_ptr<node::Options> NodeLocalCacheTest::opt = nullptr;
std::unique_ptr<node::LocalCachedInterface> NodeLocalCacheTest::cache = nullptr;
std::vector<std::string> NodeLocalCacheTest::events{};


const std::string DATA_TAG = "d";
const std::string TAKED_TAG = "t";

TEST_F(NodeLocalCacheTest, HandleOptionsReturn) {
  EXPECT_EQ(4, opt->get_batch_size());
  // EXPECT_EQ(50, opt->get_cache_size());
  // EXPECT_EQ("0.0.0.0", opt->get_listen_addr());
  EXPECT_EQ("/tmp/nodecache", opt->get_wal_path());
}

TEST_F(NodeLocalCacheTest, HandleAddEvents) {

  for (const auto e : events) {
    EXPECT_TRUE(cache->add(e)) << "add eventdata to cache: " << e ;
  }

  EXPECT_EQ(10, cache->get_range_count(DATA_TAG)) << "should have 10*3 + 5 job +node relations";
}

TEST_F(NodeLocalCacheTest, HandleGetEvents) {

  std::string data;
  EXPECT_TRUE(cache->get(&data)) << "get eventdata from cache.";

  EXPECT_EQ("d5", cache->get_limit_key());

  std::cout << "data: " << data << std::endl;

  EXPECT_TRUE(cache->get(&data)) << "get eventdata from cache.";

  EXPECT_EQ("d9", cache->get_limit_key());

  data.clear();
  EXPECT_TRUE(cache->get(&data)) << "get eventdata from cache.";

  EXPECT_EQ("e", cache->get_limit_key());
  std::cout << "data: " << data << std::endl;

  EXPECT_EQ(10, cache->get_range_count(DATA_TAG)) << "should have 10 job +node relations";
}

TEST_F(NodeLocalCacheTest, HandleDelEvent) {
  EXPECT_TRUE(cache->recovery()) << "get eventdata from cache.";
  EXPECT_EQ("d", cache->get_limit_key());

  std::string data;
  EXPECT_TRUE(cache->get(&data)) << "get eventdata from cache.";
  EXPECT_EQ("d5", cache->get_limit_key());

  EXPECT_TRUE(cache->del_last_get()) << "get eventdata from cache.";
  EXPECT_EQ(6, cache->get_range_count(DATA_TAG)) << "should have 10 - 4 job +node relations";

  EXPECT_TRUE(cache->del_last_get()) << "get eventdata from cache.";
  EXPECT_EQ(6, cache->get_range_count(DATA_TAG)) << "should have 10 - 4 job +node relations";

  EXPECT_TRUE(cache->get(&data)) << "get eventdata from cache.";
  EXPECT_EQ("d9", cache->get_limit_key());
  EXPECT_TRUE(cache->get(&data)) << "get eventdata from cache.";
  EXPECT_TRUE(cache->del_last_get()) << "get eventdata from cache.";
  EXPECT_EQ(0, cache->get_range_count(DATA_TAG)) << "should have 10 - 10 job +node relations";
  EXPECT_EQ("d", cache->get_limit_key());
}
