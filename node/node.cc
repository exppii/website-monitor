//
// Created by 刘优 on 16/6/3.
//

#include <iostream>
#include "node/node_server.h"
#include "common/options.h"
#include "common/utils.h"
#include "node/logger.h"

using namespace std;
using namespace webmonitor;

int main(int argc, char const *argv[]) {

  Options opt;

  //create logs dir to save log files.
  mkdir_if_not_exists(opt.get_log_path());

  //init log config
  init_logger(opt.get_log_config_path(), node::NODE_TAG);

  auto server = node::NodeServerUniquePtr(&opt);

  auto logger = spdlog::get(node::NODE_TAG);

  try {
    server->start();
  }catch (const std::exception& ex) {
    logger->error("Exception happened: ") << ex.what();
    return 1;
  } catch (...) {
    logger->error("Unkown error happened.");
    return 2;
  }
  return 0;
}