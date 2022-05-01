cmake_minimum_required(VERSION 3.20)
project(lab4-server)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab4-server main.cpp client-handler.h clients-db.h ../shared/utils.h ../shared/pipe-message.h)
