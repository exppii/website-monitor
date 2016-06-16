//
// Created by 刘优 on 16/6/13.
//

#include "test/node_client_task_test.h"



int main(int argc, char *argv[]) {

  // testing::AddGlobalTestEnvironment(new LocalCacheEnvironment);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}