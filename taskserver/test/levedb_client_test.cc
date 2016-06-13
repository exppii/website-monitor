//
// Created by 刘优 on 16/6/8.
//
#include <iostream>
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include "common/varint.h"
#include <climits>

using namespace std;

#include <string>


class A {

public:
  const string get_content() const {
    return _content;
  }

  void set_content(const string& content) {
    _content = content;
  }

  int64_t get_id() const {
    return _id;
  }

  void set_id(int64_t id) {
    _id = id;
  }

private:
  std::string _content;
  int64_t _id;
};


int main(int argc, char const *argv[]) {

  uint64_t st = 0;
  uint64_t ed = std::numeric_limits<uint64_t>::max();
  uint64_t x = 100232;



  char c[10] = {0};

  memset(c,0, 10);

  webmonitor::encode_fixint64(c,ed);

  const std::string PRE = "j";

  const std::string KEY = PRE + std::string(c,8) + "18";

  leveldb::Slice key(KEY);

  auto l = key.data();


  cout  << l << "  --- " << key.size() << key[0]  << endl;
  auto y = webmonitor::decode_fixed64(l + 1);
  auto r = webmonitor::decode_fixed64(l + 9);
  cout << y << endl;

  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;

  leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
  assert(status.ok());

  std::string value{"hello world"};
  std::string key1{"hello"};
  std::string key2{"hello2"};
  auto s = db->Put(leveldb::WriteOptions(), key1, value);
  if(s.ok()) cout << "insert ok" <<endl;

  std::string value2;
  if (s.ok()) db->Get(leveldb::ReadOptions(), key1, &value2);

  cout << "get value: " << value2 << endl;

  if (s.ok()) {
    leveldb::WriteBatch batch;

    batch.Put(key1, value);
    batch.Delete(key2);
    s = db->Write(leveldb::WriteOptions(), &batch);

  }
  if(s.ok()) cout << "batch ok" <<endl;
  else cout << s.ToString() << endl;

  s = db->Delete(leveldb::WriteOptions(),key2);
  if(s.ok()) cout << "de ok" <<endl;
  else cout << s.ToString() << endl;
  delete db;
  return 0;
}