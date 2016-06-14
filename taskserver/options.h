//
// Created by 刘优 on 16/6/4.
//

#ifndef WEBSITEMONITOR_TASKSERVER_COMMON_OPTIONS_H_
#define WEBSITEMONITOR_TASKSERVER_COMMON_OPTIONS_H_

#include <string>

namespace webmonitor {

namespace taskserver {

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

} //namespace taskserver

} //namespace webmonitor

#endif //WEBSITEMONITOR_TASKSERVER_COMMON_OPTIONS_H_
