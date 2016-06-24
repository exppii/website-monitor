//
// Created by 刘优 on 16/6/23.
//
#include <leveldb/slice.h>
#include <cstring>
#include <iostream>

void encode_fixint64(char* buf, uint64_t value) {
  std::memcpy(buf, &value, sizeof(value));
}

uint64_t decode_fixint64(const char* buf ) {
  uint64_t result{0};
  std::memcpy(&result, buf, sizeof(result));
  return  result;
}

std::string id_to_string( uint64_t id) {
  char buf[8] = {0};
  encode_fixint64(buf,id);
  return std::string(buf,8);
}

leveldb::Slice add(const leveldb::Slice& a) {
  auto c = decode_fixint64(a.data() +1); // offset to fixint
  leveldb::Slice ret{"d" + id_to_string(++c)};
  std::cout << "ret: " << decode_fixint64(ret.data() + 1) << std::endl;
  return ret;
}

std::string add_one(const std::string& a) {
  auto c = decode_fixint64(a.data() +1); // offset to fixint
  std::string ret{"d" + id_to_string(++c)};
  std::cout << "ret: " << decode_fixint64(ret.data() + 1) << std::endl;
  return ret;
}

int main(int argc, char const *argv[]) {

  leveldb::Slice a{"d" + id_to_string(10)};

  auto b = add(a);
  std::cout << "d";
  std::cout << "a: " << decode_fixint64(a.data() + 1) << std::endl;
  std::cout << "b: " << decode_fixint64(b.data() + 1) << std::endl;


  std::string c{"d" + id_to_string(10)};

  auto d = add_one(c);
  std::cout << "c: " << decode_fixint64(c.data() + 1) << std::endl;
  std::cout << "d: " << decode_fixint64(d.data() + 1) << std::endl;


}