//
// Created by 刘优 on 16/6/13.
//

#include "common/utils.h"
//#include "node/options.h"
#include "gtest/gtest.h"


TEST(file_existsTest, HandleTrueReturn) {
  EXPECT_TRUE(file_exists("../conf/node_service.config"));
  EXPECT_TRUE(file_exists("../conf/node_log.config"));
}