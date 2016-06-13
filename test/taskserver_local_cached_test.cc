//
// Created by 刘优 on 16/6/13.
//

#include "common/utils.h"
#include "gtest/gtest.h"

TEST(file_existsTest, HandleTrueReturn) {
  EXPECT_TRUE(file_exists("../conf/taskserver.config"));
  EXPECT_TRUE(file_exists("../conf/taskserver_log.config"));
}