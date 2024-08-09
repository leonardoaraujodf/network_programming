cmake_minimum_required(VERSION 3.22)
project(unix_list)

set(SOURCE_FILES
    net_adapters.cpp
    unix_list.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
