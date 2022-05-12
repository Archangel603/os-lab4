cmake_minimum_required(VERSION 3.20)
project(lab4-server)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab4-server main.cpp client-handler.h clients-db.h ../shared/utils.h ../shared/pipe-message.h)
#target_include_directories(lab4-server PRIVATE "C:/Program Files/MySQL/MySQL Server 8.0/include")
#target_include_directories(lab4-server PRIVATE "C:/Program Files/MySQL/MySQL Server 8.0/lib")
#target_link_libraries(lab4-server "C:/Program Files/MySQL/MySQL Server 8.0/lib/libmysql.lib")
target_include_directories(lab4-server PRIVATE "C:/Program Files/MySQL/Connector C++ 8.0/include/jdbc/")
target_include_directories(lab4-server PRIVATE "C:/Program Files/MySQL/Connector C++ 8.0/lib64/")
target_link_libraries(lab4-server "C:/Program Files/MySQL/Connector C++ 8.0/lib64/vs14/mysqlcppconn.lib")
