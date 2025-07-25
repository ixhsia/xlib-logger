cmake_minimum_required(VERSION 3.30)
project(log_xlib VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

file(GLOB SRC_FILES src/*.cpp src/logger/*.cpp)
add_library(log_xlib STATIC ${SRC_FILES})
target_include_directories(log_xlib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/log_xlib>
        $<INSTALL_INTERFACE:include>
)

add_subdirectory(test)
