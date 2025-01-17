#include "../include/remote_control.h"
#include <iostream>
#include <fstream>

json readConfig(const string& config_path)
{
    ifstream config_file(config_path);
    if (!config_file.is_open())
    {
        cerr << "unable to find config\n";
    }
    json config;
    config_file >> config;
    return config;
}

void writeConfig(const string& config_path, const json& new_config)
{
    ofstream previous_config(config_path);
    if (!previous_config.is_open())
    {
        cerr << "unable to find config\n";
    }
    previous_config << new_config.dump(4);
}

void httpCommunicate(const string& path,httplib::Server& server) {
    //设置CORS头，处理跨域请求
    auto setCORSHeaders=[](httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    };

    //预检请求
    server.Options("/config", [setCORSHeaders](const httplib::Request& req, httplib::Response& res)
    {
        setCORSHeaders(res);
        res.status = 204;
    });

    //获取配置文件
    server.Get("/config", [path,setCORSHeaders](const httplib::Request& req, httplib::Response& res)
    {
        try
        {
            json config = readConfig(path);
            setCORSHeaders(res);
            res.set_content(config.dump(), "application/json");
        }
        catch (const exception& err)
        {
            setCORSHeaders(res);
            res.status = 500;
            res.set_content("error: " + string(err.what()), "text/plain");
        }
    });

    //修改配置文件
    server.Post("/config", [path,setCORSHeaders](const httplib::Request& req, httplib::Response& res)
    {
        json new_config = json::parse(req.body);
        writeConfig(path, new_config);
        setCORSHeaders(res);
    });
    server.listen("0.0.0.0", 9002);
}