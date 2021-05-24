cmake_minimum_required(VERSION 3.17)
project(EasyXTest)

set(CMAKE_CXX_STANDARD 14)

add_executable(EasyXTest main.cpp playerFighter.h Enemy.h bullet.h model.h timer.h)