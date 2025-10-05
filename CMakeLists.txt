cmake_minimum_required(VERSION 3.17)
project(EasyXTest)

set(CMAKE_CXX_STANDARD 14)

# 设置 EasyX 库路径
set(EASYX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/easyx)
include_directories(${EASYX_DIR}/include)
link_directories(${EASYX_DIR}/lib64)

add_executable(EasyXTest main.cpp model.h checkpoint.h)

# 链接 EasyX 库和系统库
target_link_libraries(EasyXTest easyx gdi32 ole32)