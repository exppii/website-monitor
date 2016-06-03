
#ifndef WEBSITE_MONITOR_INCLUDE_JOB_H_
#define  WEBSITE_MONITOR_INCLUDE_JOB_H_

#include <string>

namespace webmonitor {

class job {

public:

  virtual void update() = 0;


private:
  std::string _id;
  int _frequency{5};
  std::string _dest;

};

}

#endif //WEBSITE_MONITOR_INCLUDE_JOB_H_
