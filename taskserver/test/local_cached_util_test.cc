//
// Created by 刘优 on 16/6/13.
//

#include <iostream>
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include "common/varint.h"

#include "taskserver/local_cache_util_interface.h"
#include "taskserver/local_cache_util.h"
#include "taskserver/options.h"

using namespace std;
using namespace webmonitor;
using namespace webmonitor::taskserver;

int main(int argc, char const *argv[]) {

  taskserver::Options opt("../conf/node_service.config");

  auto cache = LevelDBCachedUtilUniquePtr("",&opt);



  return 0;
}