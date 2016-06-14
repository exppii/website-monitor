

#include "taskserver_local_cached_test.h"

int main(int argc, char *argv[]) {

   // testing::AddGlobalTestEnvironment(new LocalCacheEnvironment);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
