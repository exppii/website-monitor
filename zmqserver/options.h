//
// Created by 刘优 on 16/6/20.
//

#ifndef WEBSITEMONITOR_ZMQSERVER_OPTIONS_H_
#define WEBSITEMONITOR_ZMQSERVER_OPTIONS_H_

#include <string>
#include <memory>

namespace webmonitor {

namespace zmqserver {

class LocalConfig;

class Options {

public:
  explicit Options(const std::string& config_path);
  ~Options();

  std::string get_log_path() const;
  std::string get_log_config_path() const;

  unsigned int get_cache_size() const;
  std::string get_listen_addr() const;
  std::string get_cachefile_path() const;
  unsigned int get_listen_port() const;

private:
  std::unique_ptr<LocalConfig> _config;
};

} //namespace zmqserver

} //namespace webmonitor

#endif //WEBSITEMONITOR_ZMQSERVER_OPTIONS_H_
