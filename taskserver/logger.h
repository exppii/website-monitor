//
// Created by 刘优 on 16/6/8.
//

#ifndef WEBSITEMONITOR_TASKSERVER_LOGGER_H_
#define WEBSITEMONITOR_TASKSERVER_LOGGER_H_

#include "spdlog/spdlog.h"

namespace webmonitor {

namespace taskserver {

const std::string SERVER_TAG = "taskserver";

bool init_logger(const std::string& path) noexcept;

}

}

#endif //WEBSITEMONITOR_TASKSERVER_LOGGER_H_
