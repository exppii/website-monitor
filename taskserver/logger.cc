//
// Created by 刘优 on 16/6/8.
//

#include "taskserver/logger.h"
#include <fstream>
#include <sstream>

#include "common/json.hpp"

#ifndef NDEBUG

#include "common/utils.h"
#include <cassert>
#endif

bool webmonitor::taskserver::init_logger(const std::string& path) noexcept {

#ifndef NDEBUG
  assert(file_exists(path));
#endif

  auto ret = false;
  std::ifstream file(path);
  if (file) {
    std::stringstream ss;
    ss << file.rdbuf();
    auto conf = nlohmann::json::parse(ss);
    auto filename = conf["file"].get<std::string>();
    auto max_file_size = conf["maxFileSize"].get<long>();
    auto max_index = conf["maxBackupIndex"].get<int>();

#ifdef NDEBUG
    auto file_logger = spdlog::rotating_logger_mt(id, filename, max_file_size, max_index,true);
#else
    auto file_logger = spdlog::rotating_logger_mt(SERVER_TAG, filename, max_file_size, max_index,true);
#endif

    spdlog::set_pattern(conf["pattern"].get<std::string>());

    std::string level = conf["level"].get<std::string>();

    int l = 0;

    do {
      if (level == spdlog::level::level_names[l]) break;
    }
    while (++l < 10);

    if(l < 10 ) {
      spdlog::set_level(static_cast<spdlog::level::level_enum>(l));
      ret = true;
    }

    file.close();
  }

  return true;
}