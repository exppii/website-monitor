#Website Monitor 


## 一、环境依赖

|依赖|版本|备注|
|:-:|:-:|:-:|
|gcc|>=4.8.x|支持c++11|
|cmake|>=2.8.x|编译工具|
|grpc|>=1.14|gRPC是由Google设计基于HTTP/2协议标准的一个高性能、通用的开源RPC框架，基于ProtoBuf序列化协议开发，且支持众多开发语言|
|protobuf|>= 3.0|Protocol Buffers 是一种轻便高效的结构化数据存储格式,用于结构化数据串行化|
| leveldb |>= 1.18|LevelDb 是 Google 开源的持久化 KV 单机存储引擎 用于维护任务列表和节点状态列表|

## 二、依赖环境安装:

####  添加必要源并安装编译工具:

```bash
sudo yum install gcc gcc-c++ autoconf cmake #编译工具
sudo yum install wget git zlib-devel libsodium-devel 
```

### 编译安装grpc 
参考[github上安装说明](https://github.com/grpc/grpc/blob/master/INSTALL.md)

```bash
cd /temp 
git clone --depth==1 https://github.com/grpc/grpc.git
cd grpc
git submodule update --init
make -j8
[sudo] make install
```

### 编译安装protobuf
参考[github上安装说明](https://github.com/google/protobuf/blob/master/src/README.md)

```bash
cd /temp 
git clone --depth==1 https://github.com/google/protobuf.git
./configure
make -j8
make check
[sudo] make install
sudo ldconfig # refresh shared library cache.
```
### 编译安装leveldb

参考[github上安装说明](https://github.com/google/leveldb)

```bash
cd /temp 
git clone --depth==1 https://github.com/google/leveldb.git
make -j8
[sudo] make install
sudo ldconfig # refresh shared library cache.
```
## 编译

```bash
mkdir build &&cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
make -j8
make test
#make install

```
