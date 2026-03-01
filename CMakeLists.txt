cmake_minimum_required(VERSION 3.21)
project(Tsensor C)

# 设置 include 目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.c)

# 添加可执行文件
add_executable(tsensor ${SRC_FILES})
