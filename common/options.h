//
// Created by 刘优 on 16/6/4.
//

#ifndef WEBSITEMONITOR_COMMON_OPTIONS_H_
#define WEBSITEMONITOR_COMMON_OPTIONS_H_

#include <string>

namespace webmonitor {

class Options {

public:
  Options() {}

  std::string get_log_path();
  std::string get_log_config_path();

};

} //namespace webmonitor

#endif //WEBSITEMONITOR_COMMON_OPTIONS_H_
