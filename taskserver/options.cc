//
// Created by 刘优 on 16/6/4.
//

#include "taskserver/options.h"

#include "common/json.hpp"
#ifndef NDEBUG

#include "common/utils.h"
#include <cassert>
#endif

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

namespace webmonitor {

namespace taskserver {

class LocalConfig {
public:
  explicit LocalConfig(const std::string &config_path) {
#ifndef NDEBUG
    assert(file_exists(config_path));
#endif
    std::string config;

    if(read_file(config_path,&config)) {
      data = nlohmann::json::parse(config);
    }
    _varify_config();
  }

private:

  //TODO varify input config args
  void _varify_config(){
    ;
  }

private:
  friend class Options;
  nlohmann::json data;

};


Options::Options(const std::string &config_path)
    :_config(make_unique<LocalConfig>(config_path)) {

}

Options::~Options() {

}

std::string Options::get_log_path() const {
  return "";
}

std::string Options::get_log_config_path() const {
  return "../conf/taskserver_log.config";
}

unsigned int Options::get_cache_size() const {
  return _config->data["cache_size"].get<unsigned int>();
}

std::string Options::get_listen_addr() const {
  return _config->data["listen_addr"].get<std::string>();
}

std::string Options::get_chachefile_path() const {
  return _config->data["chachefile_path"].get<std::string>();
}

unsigned int Options::get_listen_port() const {
  return _config->data["listen_port"].get<unsigned int>();
}

} //namespace taskserver

} //namespace webmonitor



