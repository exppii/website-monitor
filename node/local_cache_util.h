//
// Created by 刘优 on 16/6/21.
//

#ifndef WEBSITEMONITOR_NODE_LOCAL_CACHE_UTIL_H_
#define WEBSITEMONITOR_NODE_LOCAL_CACHE_UTIL_H_

#include <string>

namespace webmonitor {

namespace node {

class Options;

class LocalCachedInterface {

public:

  virtual ~LocalCachedInterface(){}

  virtual bool add(const std::string& data) = 0;

  virtual bool get(const uint64_t handle,std::string* data) = 0;

  virtual bool del(const uint64_t& handle) = 0;

};

LocalCachedInterface* NewLevelDBCachedPtr(const Options*);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_LOCAL_CACHE_UTIL_H_
