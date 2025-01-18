#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <string>
#include <mutex>
#include <opencv2/opencv.hpp>
using namespace std;
using json = nlohmann::json;
using namespace cv;

// extern Mat current_frame;
// extern mutex frame_mutex;

//读取配置文件
json readConfig(const string& config_path);

//写入配置文件
void writeConfig(const string& config_path, const json& new_config);

//设置CORS头，用于跨域请求
void setCORSHeaders(httplib::Response &res);

//解码Mat格式
vector<uchar> encodeImage(const Mat& image);

//处理前端修改配置请求
void httpConfigModification(const string& path,httplib::Server &server);

//处理前端视频流请求
void httpVideoStream(httplib::Server &server,Mat& current_frame,mutex& frame_mutex);

#endif //REMOTE_CONTROL_H
