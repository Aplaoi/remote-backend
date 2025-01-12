cmake_minimum_required(VERSION 3.10)
project(backend)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(
        test
        main.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/cpp-httplib)

add_subdirectory(libs/cpp-httplib)
target_link_libraries(test httplib)