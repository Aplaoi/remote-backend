#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <string>
using namespace std;
using json = nlohmann::json;

//读取配置文件
json readConfig(const string& config_path);

//写入配置文件
void writeConfig(const string& config_path, const json& new_config);



//处理前端请求
void httpCommunicate(const string& path,httplib::Server &server);

#endif //REMOTE_CONTROL_H
