cmake_minimum_required(VERSION 3.22)
project(time_server)

set(SOURCE_FILES
    time_server.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++17 -Wall -Wextra -g)
