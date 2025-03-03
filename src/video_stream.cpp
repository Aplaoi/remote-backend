#include "httplib.h"
#include "../include/remote_control.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <string>
const std::string BOUNDARY = "--frame_boundary";

// 编码图像函数
std::vector<uchar> encodeImage(const cv::Mat& image, int quality) {
    std::vector<uchar> buffer;
    if (image.empty()) return buffer;
    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, quality};
    if (!cv::imencode(".jpg", image, buffer, params)) {
        std::cerr << "Image encoding failed." << std::endl;
    }
    return buffer;
}

// HTTP 视频流处理函数
void httpVideoStream(httplib::Server& server, cv::Mat& current_frame, std::mutex& frame_mutex) {
    server.Get("/video", [&current_frame, &frame_mutex](const httplib::Request& req, httplib::Response& res) {
        int quality = 15;
        int width = 320;
        int height = 180;
        int fps = 15;

        const int interval_ms = 1000 / fps;

        res.set_header("Content-Type", "multipart/x-mixed-replace; boundary=" + BOUNDARY);
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Cache-Control", "no-store");

        std::string part_header;
        part_header.reserve(100);

        res.set_chunked_content_provider("",[&](size_t offset, httplib::DataSink &sink) {
            bool first_part = true;
            while (true) {
                cv::Mat frame;
                {
                    std::lock_guard<std::mutex> lock(frame_mutex);
                    if (current_frame.empty()) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        continue;
                    }
                    frame = current_frame;
                }

                if (width > 0 && height > 0) {
                    cv::resize(frame, frame, cv::Size(width, height));
                }

                auto buffer = encodeImage(frame, quality);
                if (buffer.empty()) continue;

               // 构造多部分头
               if (first_part) {
                   part_header = BOUNDARY + "\r\n";
                   first_part = false;
               } else {
                   part_header = "\r\n" + BOUNDARY + "\r\n";
               }
               part_header += "Content-Type: image/jpeg\r\n"
                              "Content-Length: " + std::to_string(buffer.size()) + "\r\n\r\n";

               // 发送数据
               if (!sink.write(part_header.data(), part_header.size())) {
                   std::cerr << "Failed to write part header." << std::endl;
                   break;
               }
                if (!sink.write(reinterpret_cast<char*>(buffer.data()), buffer.size())) {
                    std::cerr << "Failed to write image data." << std::endl;
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
            }
            sink.done();
            return true;
        });
    });
}
