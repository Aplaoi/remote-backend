#include <iostream>
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
using namespace std;
using json = nlohmann::json;
#include "include/remote_control.h"

int main()
{
    httplib::Server server;
    const string default_config_path = "/mnt/windows/local_cpp/remote-backend/config.json";
    json config=readConfig(default_config_path);
    int fps = config["Debug"]["ImageThread"]["FPS"];
    bool light_flag = config["Debug"]["ImageThread"]["Light"];
    double scale = config["Debug"]["ImageThread"]["Scale"];
    int delay = 1000 / fps;
    httpCommunicate(default_config_path,server);
    cout << "Server is running on 0.0.0.0:9002\n";
}
