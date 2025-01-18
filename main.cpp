#include <iostream>
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;
using json = nlohmann::json;
#include "include/remote_control.h"
#include <opencv2/opencv.hpp>

void serverListen(httplib::Server &server) {
    server.listen("0.0.0.0",9002);
    cout << "Server is running on 0.0.0.0:9002\n";
}

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
    Mat current_frame;
    mutex frame_mutex;
    httpVideoStream(server, current_frame, frame_mutex);
    thread server_thread(serverListen,ref(server));
    while (true)
    {
        Mat frame;
        cap >> frame;
        if (frame.empty()){
            cout<<"fail to fetch the frame from camera\n";
            continue;
        }
        lock_guard<mutex> lock(frame_mutex);
        current_frame=frame.clone();
        
        this_thread::sleep_for(chrono::milliseconds(33));
    }
    server.stop();
    server_thread.join();
    cout << "Server stopped\n";
    return 0;
}
