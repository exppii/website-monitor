//
// Created by 刘优 on 16/6/13.
//

#include "node/options.h"
#include "node/local_cache_util.h"
#include <sstream>

#include "gtest/gtest.h"

using namespace webmonitor;

class NodeLocalCacheTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {
    opt.reset(new node::Options("../conf/node_local_cache_test.config"));
    cache.reset(node::NewLevelDBCachedPtr(opt.get()));
    _init_event_list();
  }

  static void TearDownTestCase() {
    const std::string command = "rm -rf " + opt->get_wal_path();
    system(command.c_str());
  }

  static void _init_event_list() {
    for (size_t i = 1; i < 11; i++) {
      std::stringstream ss;
      ss << R"({"job_id" : )" << i << ",";
      ss << R"("data" : )" << "rawdata" << i << "}";
      events.push_back(ss.str());
    }
  }


  static std::unique_ptr<node::Options> opt;
  static std::unique_ptr<node::LocalCachedInterface> cache;
  static std::vector<std::string> events;
};
