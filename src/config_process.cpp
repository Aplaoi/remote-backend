#include "../include/remote_control.h"
#include <iostream>
#include <fstream>

void httpConfigModification(const string &path, httplib::Server &server) {
    //POST前需预检，保证满足CORS policy
    server.Options("/config", [](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST,PUT,DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
        res.status = 204;
    });

    //获取配置文件
    server.Get("/config", [path](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        try {
            ifstream config_file(path);
            if (!config_file.is_open()) {
                cerr << "unable to find config\n";
            }
            json config;
            config_file >> config;
            res.set_content(config.dump(), "application/json");
        } catch (const exception &err) {
            res.status = 500;
            res.set_content("error: " + string(err.what()), "text/plain");
        }
    });

    //修改配置文件
    server.Post("/config", [path](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        // res.set_header("Access-Control-Allow-Methods", "GET, POST,PUT,DELETE, OPTIONS");
        // res.set_header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
        // res.set_header("Content-Type", "application/json");
        json new_config = json::parse(req.body);
        ofstream previous_config(path);
        if (!previous_config.is_open()) {
            cerr << "unable to find config\n";
        }
        previous_config << new_config.dump(4);
    });
}
