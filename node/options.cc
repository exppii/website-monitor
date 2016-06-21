//
// Created by 刘优 on 16/6/4.
//

#include "node/options.h"

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

namespace node {

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
  return "../conf/node_log.config";
}

uint32_t Options::get_report_frequency() const {
  return _config->data["report_frequency"].is_number() ? _config->data["report_frequency"].get<uint32_t>() : 3;

}

uint32_t Options::get_fetchtask_frequency() const {
  return _config->data["fetchtask_frequency"].is_number() ? _config->data["fetchtask_frequency"].get<uint32_t>() : 1;
}

uint32_t Options::get_taskserver_port() const {
  return _config->data["taskserver_port"].get<uint32_t>();
}

std::string Options::get_taskserver_addr() const {
  return _config->data["taskserver_addr"].get<std::string>();
}

std::string Options::get_upload_addr() const {
  return _config->data["upload_addr"].get<std::string>();
}

uint32_t Options::get_upload_port() const {
  return _config->data["upload_port"].get<uint32_t>();
}

uint64_t Options::get_node_id() const {
  return _config->data["node_id"].get<uint64_t>();
}

std::string Options::get_node_name() const {
  return _config->data["node_name"].get<std::string>();
}

std::string Options::get_encrypt() const {
return _config->data["encrypt"].get<std::string>();
}

std::string Options::get_compress() const {
return _config->data["compress"].get<std::string>();
}

std::string Options::get_wal_path() const {
return _config->data["wal_path"].get<std::string>();
}

uint32_t Options::get_batch_size() const {
  return _config->data["batch_size"].get<uint32_t>();
}

} //namespace node

} //namespace webmonitor
