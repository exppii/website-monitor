//
// Created by 刘优 on 16/6/12.
//

#include "common/varint.h"

#include "common/port_posix.h"

namespace webmonitor {



void encode_fixint32(char* buf, uint32_t value) {

  if (port::kLittleEndian) {

    memcpy(buf, &value,sizeof(value));
  } else {
    auto s = sizeof(value);

    for (int i = 0; i != s; ++i) {
      buf[i] = (value >> i*8)& 0xff;
    }
  }
}

void encode_fixint64(char* buf, uint64_t value) {

  if (port::kLittleEndian) {

    memcpy(buf, &value,sizeof(value));
  } else {
    auto s = sizeof(value);

    for (int i = 0; i != s; ++i) {
      buf[i] = (value >> i*8)& 0xff;
    }
  }

}


//char* encode_varint32(char* dst, uint32_t value) {
//
//}
//char* encode_varint64(char* dst, uint64_t vaule) {
//
//}

uint32_t decode_fixed32(const char* ptr) {
  if (port::kLittleEndian) {

    uint32_t result;

    memcpy(&result, ptr,sizeof(result));  // gcc optimizes this to a plain load

    return result;

  } else {

    return((static_cast<uint32_t>(static_cast<unsigned char>(ptr[0])))

           |(static_cast<uint32_t>(static_cast<unsigned char>(ptr[1])) <<8)

           | (static_cast<uint32_t>(static_cast<unsigned char>(ptr[2])) << 16)

           |(static_cast<uint32_t>(static_cast<unsigned char>(ptr[3])) <<24));

  }
}
uint64_t decode_fixed64(const char* ptr) {
  if (port::kLittleEndian) {

    uint64_t result;

    memcpy(&result, ptr,sizeof(result));  // gcc optimizes this to a plain load

    return result;

  } else {

    return((static_cast<uint64_t>(static_cast<unsigned char>(ptr[0])))
           |(static_cast<uint64_t>(static_cast<unsigned char>(ptr[1])) <<8)
           |(static_cast<uint64_t>(static_cast<unsigned char>(ptr[2])) << 16)
           |(static_cast<uint64_t>(static_cast<unsigned char>(ptr[3])) <<24)
           |(static_cast<uint64_t>(static_cast<unsigned char>(ptr[4])) << 32)
           |(static_cast<uint64_t>(static_cast<unsigned char>(ptr[5])) << 40)
           | (static_cast<uint64_t>(static_cast<unsigned char>(ptr[6])) << 48)
           | (static_cast<uint64_t>(static_cast<unsigned char>(ptr[7])) << 56));

  }
}

}