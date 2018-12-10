cmake_minimum_required(VERSION 3.13)
project(DVR)

set(CMAKE_CXX_STANDARD 17)

add_executable(Router router.cpp Socket.h Utils.h RouterDS.h)
