#include "../include/remote_control.h"
#include <vector>
#include <opencv2/opencv.hpp>

vector<uchar> encodeImage(const Mat& image){
    vector<uchar> buffer;
    imencode(".jpg",image,buffer);
    return buffer;
}

void httpVideoStream(const Mat& frame,httplib::Server &server){
    server.Get("/video",[frame](const httplib::Request& req,httplib::Response& res){
        auto jpg=encodeImage(frame);
        setCORSHeaders(res);
        res.set_content(reinterpret_cast<char*>(jpg.data()), jpg.size(), "image/jpeg");
    });
    
}