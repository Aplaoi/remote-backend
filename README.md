# 服务端

## 依赖模块

本项目使用了cpp-httplib和nlonmann_json的外部库，需在libs文件夹内拷贝库

```bash
mkdir libs
cd libs
git clone git@github.com:yhirose/cpp-httplib.git
git clone git@github.com:nlohmann/json.git
```

## 项目结构

```bash
remote-backend/
├── CMakeLists.txt	
├── config.json				# json配置文件，用于测试
├── main.cpp				
├── include/
│   └── remote_control.h	 # 函数接口
└── src/
    ├── config_process.cpp	 # 处理配置文件
    └── video_stream.cpp	 # 处理视频流
```

## 函数接口说明


### `vector<uchar> encodeImage(const Mat& image)`

- 用途：将 `Mat` 类型的图像数据编码为 JPEG 格式，并返回编码后的字节流。
- 参数
  - `image`：要编码的图像，类型为 `Mat`。

### `void httpConfigModification(const string& path,httplib::Server &server)`

- **用途**：为 `httplib::Server` 对象添加处理配置文件相关请求

- 参数

  - `path`：配置文件的路径，类型为 `string`。
  - `server`：`httplib::Server` 对象

  添加的路由包括：

  - `OPTIONS /config`：处理预检请求，设置 CORS 头信息并返回状态码 204，主要是POST前处理跨域 CORS 请求
  - `GET /config`：读取配置文件并以 JSON 格式返回配置信息。如果读取失败，返回状态码 500 和错误信息。
  - `POST /config`：解析请求体中的 JSON 数据并写入配置文件

### `void httpVideoStream(httplib::Server &server,Mat& current_frame,mutex& frame_mutex)`

- **用途**：为 `httplib::Server` 对象添加处理视频流请求
- 参数
  - `server`：`httplib::Server` 对象
  - `current_frame`：当前帧的图像数据，类型为 `Mat`。
  - `frame_mutex`：互斥锁，用于保护 `current_frame` 的访问，类型为 `mutex`。
- 当客户端请求 `/video` 路径时，会返回当前帧的 JPEG 图像数据
