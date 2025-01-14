cmake_minimum_required(VERSION 3.10)
project(remote-backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加子模块
add_subdirectory(libs/json)
add_subdirectory(libs/cpp-httplib)

# 添加可执行文件
add_executable(${PROJECT_NAME} main.cpp)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE httplib nlohmann_json::nlohmann_json)