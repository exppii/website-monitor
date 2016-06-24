//
// Created by 刘优 on 16/6/23.
//

#include "test/node_client_local_cache_test.h"

#include <iostream>

std::unique_ptr<node::Options> NodeLocalCacheTest::opt = nullptr;
std::unique_ptr<node::LocalCachedInterface> NodeLocalCacheTest::cache = nullptr;
std::vector<std::string> NodeLocalCacheTest::events{};


const std::string DATA_TAG = "d";

TEST_F(NodeLocalCacheTest, HandleOptionsReturn) {
  EXPECT_EQ(4, opt->get_batch_size());
  EXPECT_EQ("/tmp/nodecache", opt->get_wal_path());
}

TEST_F(NodeLocalCacheTest, HandleAddEvents) {

  for (const auto e : events) {
    EXPECT_TRUE(cache->add(e)) << "add eventdata to cache: " << e ;
  }

  EXPECT_EQ(10, cache->get_range_count(DATA_TAG)) << "should have 10 event data.";
}

TEST_F(NodeLocalCacheTest, HandleGetEvents) {
  std::string data;
  EXPECT_TRUE(cache->get(&data)) << "get eventdata from cache.";

  EXPECT_EQ("d4", cache->get_end_key());
  EXPECT_EQ("d5", cache->get_start_key());

  data.clear();
  EXPECT_TRUE(cache->get(&data)) << "get eventdata from cache.";

  EXPECT_EQ("d8", cache->get_end_key());
  EXPECT_EQ("d9", cache->get_start_key());
//
  EXPECT_TRUE(cache->get(&data)) << "get eventdata from cache.";
//f
  EXPECT_EQ("d10", cache->get_end_key());
  EXPECT_EQ("d11", cache->get_start_key());
//
  EXPECT_EQ(10, cache->get_range_count(DATA_TAG)) << "should have 10 job +node relations";
}

TEST_F(NodeLocalCacheTest, HandleDelEvent) {
  EXPECT_TRUE(cache->recovery()) << "get eventdata from cache.";
  EXPECT_EQ("d8", cache->get_end_key());
  EXPECT_EQ("d9", cache->get_start_key());
  EXPECT_TRUE(cache->del_last_get()) << "get eventdata from cache.";

  EXPECT_EQ(2, cache->get_range_count(DATA_TAG)) << "should have 10 - 8 job +node relations";

  std::string data;
  EXPECT_TRUE(cache->get(&data)) << "get eventdata from cache.";
  EXPECT_EQ("d10", cache->get_end_key());
  EXPECT_EQ("d11", cache->get_start_key());
  EXPECT_TRUE(cache->del_last_get()) << "get eventdata from cache.";
  EXPECT_EQ(0, cache->get_range_count(DATA_TAG)) << "should have 2 - 2 job +node relations";

  EXPECT_FALSE(cache->get(&data)) << "get eventdata from cache.";
  EXPECT_EQ("d10", cache->get_end_key());
  EXPECT_EQ("d11", cache->get_start_key());
}
