cmake_minimum_required(VERSION 3.10)
project(remote-backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加子模块
add_subdirectory(libs/json)
add_subdirectory(libs/cpp-httplib)

set(OpenCV_DIR "/usr/local/opencv4.5.4/lib/cmake/opencv4")
find_package(OpenCV 4.5.4 REQUIRED)
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_calib3d)

# 添加可执行文件
add_executable(${PROJECT_NAME}
        main.cpp
        src/config_process.cpp
        src/video_stream.cpp
        include/remote_control.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})

# 链接库
target_link_libraries(${PROJECT_NAME} 
        PRIVATE 
                httplib 
                nlohmann_json::nlohmann_json
                ${OpenCV_LIBS})