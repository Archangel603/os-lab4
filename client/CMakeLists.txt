cmake_minimum_required(VERSION 3.20)
project(lab4-client)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab4-client main.cpp ../shared/pipe-message.h ../shared/utils.h)
