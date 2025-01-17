#include <iostream>
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
using namespace std;
using json = nlohmann::json;
#include "include/remote_control.h"
#include <opencv2/opencv.hpp>

int main()
{
    httplib::Server server;
   
    const string default_config_path = "/mnt/windows/local_cpp/remote-backend/config.json";
    httpConfigModification(default_config_path,server);
    VideoCapture cap(0);
    if (!cap.isOpened()){
        cerr<<"unable to open the camera\n";
        return -1;
    }
    while (true)
    {
        Mat frame;
        cap >> frame;
        httpVideoStream(frame,server);
    }
    server.listen("0.0.0.0",9002);
    cout << "Server is running on 0.0.0.0:9002\n";
}
