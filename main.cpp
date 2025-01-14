#include <iostream>
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
using namespace std;
using json = nlohmann::json;

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


int main()
{
    httplib::Server server;
    const string default_config_path = "/home/apl/cpp/remote-backend/config.json";
    server.Options("/config", [](const httplib::Request& req, httplib::Response& res)
    {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.status = 204;
    });

    server.Get("/config", [default_config_path](const httplib::Request& req, httplib::Response res)
    {
        try
        {
            json config = readConfig(default_config_path);
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            res.set_content(config.dump(), "application/json");
        }
        catch (const exception& err)
        {
            res.status = 500;
            res.set_content("error" + string(err.what()), "text/plain");
        }
    });

    cout << "Server is running on 0.0.0.0:9002\n";
    server.listen("0.0.0.0", 9002);
}


