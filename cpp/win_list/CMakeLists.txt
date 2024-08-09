cmake_minimum_required(VERSION 3.22)
project(win_list)

# Compiler flags
set(CXXFLAGS "-Wall -Wextra")

# Libraries
# set(LIBS "-liphlpapi -lws2_32")

# Setting Compiler
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
# set(CMAKE_EXE_LINKER_FLAGS "${LIBS}")

set(SOURCE_FILES
    net_adapters.cpp
    win_list.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} wsock32 ws2_32 iphlpapi)
