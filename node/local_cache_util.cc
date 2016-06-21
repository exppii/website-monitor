//
// Created by 刘优 on 16/6/21.
//

#include "node/local_cache_util.h"

#include "node/options.h"

namespace webmonitor {

namespace node {



class LevelDBCached : public LocalCachedInterface {

public:

  explicit LevelDBCached(const Options*);

  bool add(const std::string& data) override;

  bool get(const uint64_t handle,std::string* data) override;

  bool del(const uint64_t& handle) override;

};

LevelDBCached::LevelDBCached(const Options* options) {

}

bool LevelDBCached::add(const std::string& data) {
  return false;
}

bool LevelDBCached::get(const uint64_t handle,std::string* data) {
  return false;
}

bool LevelDBCached::del(const uint64_t& handle) {
  return false;
}

LocalCachedInterface* NewLevelDBCachedPtr(const Options* options) {
  return new LevelDBCached(options);
}

} //namespace node

} //namespace webmonitor
