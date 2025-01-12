#include <iostream>
#include "httplib.h"
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::string message;
std::mutex mtx;
std::condition_variable cv;
bool message_ready = false;

void inputThread() {
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        std::lock_guard<std::mutex> lock(mtx);
        message = input;
        message_ready = true;
        cv.notify_one();
    }
}

int main() {
    httplib::Server svr;
    std::thread input(inputThread);

    svr.Get("/message", [](const httplib::Request& req, httplib::Response& res) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return message_ready; });
        res.set_content(message, "text/plain");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        message_ready = false;
    });

    std::cout << "Server is running on port 8080..." << std::endl;
    svr.listen("0.0.0.0", 8080);
    input.join();
    return 0;
}