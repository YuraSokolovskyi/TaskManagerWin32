cmake_minimum_required(VERSION 3.23)
project(taskManager)

set(CMAKE_CXX_STANDARD 14)

add_executable(taskManager main.cpp prototypes.h Task.h Task.cpp)
