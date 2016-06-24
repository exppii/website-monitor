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

  virtual bool get(std::string* data) = 0;

  virtual bool del_last_get() = 0;

  virtual bool recovery() = 0;

#ifndef NDEBUG
  virtual uint64_t get_range_count(const std::string& range) = 0;

  virtual std::string get_end_key() const = 0;
  virtual std::string get_start_key() const = 0;

  //virtual void printkey() const = 0;
#endif

};

LocalCachedInterface* NewLevelDBCachedPtr(const Options*);

} //namespace node

} //namespace webmonitor

#endif //WEBSITEMONITOR_NODE_LOCAL_CACHE_UTIL_H_
