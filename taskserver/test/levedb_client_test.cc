//
// Created by 刘优 on 16/6/8.
//
#include <iostream>
#include <leveldb/db.h>

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

  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;

  leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
  assert(status.ok());

  std::string value{"hello world"};
  std::string key1{"hello"};
  auto s = db->Put(leveldb::WriteOptions(), key1, value);

  std::string value2;
  if (s.ok()) db->Get(leveldb::ReadOptions(), key1, &value2);

  cout << "get value" << value2 << endl;

  db->Delete(leveldb::WriteOptions(), key1);

  delete db;
  return 0;
}