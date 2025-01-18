#include "httplib.h"
#include "../include/remote_control.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include <mutex>

std::vector<uchar> encodeImage(const cv::Mat& image) {
    std::vector<uchar> buffer;
    if (image.empty()) {
        std::cerr << "Input image is empty\n";
        return buffer;
    }
    if (!cv::imencode(".jpg", image, buffer)) {
        std::cerr << "Failed to encode image\n";
    }
    return buffer;
}

void httpVideoStream(httplib::Server& server, cv::Mat& current_frame, std::mutex& frame_mutex) {
    server.Get("/video", [&current_frame, &frame_mutex](const httplib::Request& req, httplib::Response& res) {
        std::lock_guard<std::mutex> lock(frame_mutex); // 加锁
        if (current_frame.empty()) {
            std::cerr << "Frame is empty, skipping encoding\n";
            return;
        }
        auto jpg = encodeImage(current_frame); // 使用最新的帧数据
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(reinterpret_cast<char*>(jpg.data()), jpg.size(), "image/jpeg");
    });
}