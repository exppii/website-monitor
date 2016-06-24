//
// Created by 刘优 on 16/6/12.
//

#ifndef WEBSITEMONITOR__COMMON_VARINT_H_
#define WEBSITEMONITOR__COMMON_VARINT_H_

#include <stdint.h>

namespace webmonitor {

void encode_fixint32(char* buf, uint32_t value);
void encode_fixint64(char* buf, uint64_t value);
//char* encode_varint32(char* dst, uint32_t value);
//char* encode_varint64(char* dst, uint64_t vaule);

uint32_t decode_fixed32(const char* ptr);
uint64_t decode_fixed64(const char* ptr);

//const char* GetVarint32Ptr(const char* p, const char* limit,  uint32_t* value);
//const char* GetVarint32PtrFallback(const char* p, const char* limit,uint32_t* value);

}



#endif //WEBSITEMONITOR__COMMON_VARINT_H_
