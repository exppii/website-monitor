//
// Created by 刘优 on 16/6/21.
//
#include "node/dataproc/process_lib.h"

#include "node/options.h"

#include "node/dataproc/add_info_process.h"
#include "node/dataproc/compress_process.h"
#include "node/dataproc/encrypt_process.h"
#include "node/dataproc/wal_process.h"
#include "node/dataproc/zmq_process.h"

#if __cplusplus < 201402L
#include "common/utils.h" //using custom make_uniue
#else
using std::make_unique;
#endif

namespace webmonitor {

namespace node {


void create_data_proc_list(const Options* options, std::vector<DataProcInterfacePtr>* procs){

  procs->push_back(DataProcInterfacePtr{NewAddInfoProcPtr(options)});
  procs->push_back(DataProcInterfacePtr{NewEncryptProcPtr(options->get_encrypt())});
  procs->push_back(DataProcInterfacePtr{NewCompressProcPtr(options->get_compress())});
  procs->push_back(DataProcInterfacePtr{NewWALProcPtr(options->get_wal_path())});
}


DataProcInterface* create_data_sender(const Options* options){
  return NewZMQProcPtr(options->get_upload_addr(),options->get_upload_port());

}

} //namespace node

} //namespace webmonitor
