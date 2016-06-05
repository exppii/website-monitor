//
// Created by 刘优 on 16/6/4.
//

#include "data_proc_service.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

namespace webmonitor {

namespace node {

class NodeDataProcService : public DataProcServiceInterface {
public:

  explicit NodeDataProcService(const Options* option){}

  bool add_data(const std::string&) override;

  void start() override ;

  void stop() override ;


};

bool NodeDataProcService::add_data(const std::string &) {
  return false;
}

void NodeDataProcService::start() {

}

void NodeDataProcService::stop() {

}


std::unique_ptr<DataProcServiceInterface> DataProcServiceUniquePtr(const Options* options) {
  return make_unique<NodeDataProcService>(options);
}
} //namespace node

} //namespace webmonitor