//
// Created by 刘优 on 16/6/2.
//

#ifndef WEBSITEMONITOR_UTILS_H
#define WEBSITEMONITOR_UTILS_H

#include <string>


bool file_exists(const std::string& path);

void mkdir_if_not_exists(const std::string& path);

std::string get_current_path();

bool read_file(const std::string& file_path, std::string* dest);

/// simple implements for perfect-forwards make_unique
#if __cplusplus < 201402L
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts &&... params) {
  return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
#endif

#endif //WEBSITEMONITOR_UTILS_H
