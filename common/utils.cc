//
// Created by 刘优 on 16/6/2.
//

#include "common/utils.h"
#include <sstream>
#include <fstream>

#include <sys/stat.h>  // for mkdir
#include <dirent.h> //for DIR


bool file_exists(const std::string &path) {
  return (access(path.c_str(), F_OK) != -1);
}

void mkdir_if_not_exists(const std::string &path) {
  if (!file_exists(path))
    mkdir(path.c_str(), 0755);
}

std::string get_current_path() {
  std::string current_absolute_path;
  char current_absolute[256];
  if (NULL == getcwd(current_absolute, 256))
    return "";

  current_absolute_path = current_absolute;
  return (current_absolute_path + "/");
}

bool read_file(const std::string &file_path, std::string *dest) {
  bool ret = false;
  std::ifstream file(file_path);
  if (file) {
    std::stringstream ss;
    ss << file.rdbuf();
    *dest = ss.str();
    ret = true;
    file.close();
  }
  return ret;
}
