//
// Created by 刘优 on 16/6/8.
//

#ifndef WEBSITEMONITOR_TASKSERVER_LOCAL_CACHE_UTIL_H_
#define WEBSITEMONITOR_TASKSERVER_LOCAL_CACHE_UTIL_H_

#include "local_cache_util_interface.h"

namespace webmonitor {

namespace taskserver {

class Options;

LocalCachedUtilInterface* NewLevelDBCachedUtilPtr(Options *);


} //namespace taskserver

} //namspace webmonitor

#endif //WEBSITEMONITOR_TASKSERVER_LOCAL_CACHE_UTIL_H_
