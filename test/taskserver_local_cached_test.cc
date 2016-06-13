//
// Created by 刘优 on 16/6/13.
//

#include "common/utils.h"
#include "taskserver/options.h"
#include "gtest/gtest.h"



TEST(file_existsTest, HandleTrueReturn) {
  EXPECT_TRUE(file_exists("../conf/taskserver.config"));
  EXPECT_TRUE(file_exists("../conf/taskserver_log.config"));
}

TEST(OptionsTest, HandleTrueReturn) {
  webmonitor::taskserver::Options options("../conf/taskserver.config");
  EXPECT_EQ(5660, options.get_listen_port());
  EXPECT_EQ(50, options.get_cache_size());
  EXPECT_EQ("0.0.0.0", options.get_listen_addr());
}