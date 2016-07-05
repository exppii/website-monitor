//
// Created by 刘优 on 16/6/27.
//

#include "node/util/ping_util.h"

#include <regex>

#include "node/response/ping_response.h"


using std::regex;
using std::smatch;
using std::regex_search;

namespace webmonitor {


namespace node {

const std::string LOSS_RE = "([\\d]{0,3})% packet loss";
const std::string TTL_TIME = "min/avg/max/[\\w]+?dev = ([\\d]+?\\.[\\d]{3})/([\\d]+?\\.[\\d]{3})/([\\d]+?\\.[\\d]{3})/";


namespace {
int executeScript(const std::string &cmd, std::string &msg);

bool parser_ttl_time(const std::string &raw, PingResponse *resp);
}


bool ping_ip(const std::string &ip, PingResponse *resp) {

  bool success = false;
  const auto cmd = "ping -c 4 " + ip;

  std::string msg{};
  if (executeScript(cmd, msg) >= 0) {

    regex reg(LOSS_RE);
    smatch mch;
    if (regex_search(msg, mch, reg)) {
      resp->loss_pct = std::atoi(mch.str(1).c_str());
      success = parser_ttl_time(msg, resp);
    }
  }
  else {
    //error
  }
  return success;

}

namespace {

int executeScript(const std::string &cmd, std::string &msg) {

  int rc = -1;

  FILE *pipe = popen(cmd.c_str(), "r");
  if (!pipe) {
    msg = "Fail to open pipe to run sys command.";
    return rc;
  }
  char buffer[128];
  msg.clear();
  while (!feof(pipe)) {
    if (fgets(buffer, 128, pipe) != nullptr)
      msg += buffer;
  }

  rc = pclose(pipe);

  if (rc == -1) {
    msg = "close pipe failed.";
  }
  else {

  }
  return rc;
}

bool parser_ttl_time(const std::string &raw,
                     PingResponse *resp) {
  regex reg(TTL_TIME);
  smatch mch;
  if (regex_search(raw, mch, reg)) {
    resp->min_msec = atof(mch.str(1).c_str());
    resp->avg_msec = atof(mch.str(2).c_str());
    resp->max_msec = atof(mch.str(3).c_str());
    return true;
  }
  return false;
}

} //namespace

} //namespace node

} //namespace webmonitor
