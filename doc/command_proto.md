# 命令对接协议

### 命令码

|分类|命令码|值|备注|
|:-:|:-:|:-:|:-:|
|协议方法(PROTO_CMD)|`HTTP`|1|http/https|
|协议方法(PROTO_CMD)|`PING`|2|ping|
|协议方法(PROTO_CMD)|`DNS`|3|dns|
|请求方法(HTTP_REQ_CMD)|`HEAD`|1|head|
|请求方法(HTTP_REQ_CMD)|`GET`|2|get|
|请求方法(HTTP_REQ_CMD)|`POST`|3|post|
|匹配方法(MATCH_CMD)|`YES`|1|匹配响应|
|匹配方法(MATCH_CMD)|`NO`|2|不匹配响应|
|匹配方法(MATCH_CMD)|`NOT_CARE`|3|不关心|


### 由平台发往任务管理器

* HTTP/HTTPS监控任务

```json
{
  "http_method" : 1,
  "resp_context" : "this is correct data",
  "match_method" : 3,
}

```
* PING监控任务

```json
{
  "pkg_num" : 4,
  "pkg_size" : 64
}
```

* DNS监控任务

```json
{
  "custom_dns" : "114.114.114.114"
}
```

### 由采集器节点获取数据

* HTTP/HTTPS监控任务

```json
{
  "node_id" : 1000,
  "job_id" : 1000,
  "create_time" : 1462279637,
  "event_count" : 1,
  "event_type" : "HTTP_HEAD_NOT_CARE",
  "curl_code" : 0,
  "respcode":302,
  "appconnect_time":0.0,
  "connect_time":0.048436,
  "namelookup_time":0.0001,
  "pretransfer_time":0.04867,
  "starttransfer_time":0.084924,
  "total_time":0.084939,
  "method" : 1,
  "data" : "real html data",
  "matched" : true
}
```

curl time

```log
curl_easy_perform()
    |
    |--NAMELOOKUP
    |--|--CONNECT
    |--|--|--APPCONNECT
    |--|--|--|--PRETRANSFER
    |--|--|--|--|--STARTTRANSFER
    |--|--|--|--|--|--TOTAL
    |--|--|--|--|--|--REDIRECT

```
