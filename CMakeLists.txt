cmake_minimum_required(VERSION 3.22)
project(chat C)

set(CMAKE_C_STANDARD 99)

add_executable(client client2.c wrap.c)
add_executable(server select_server.c wrap.c)
add_executable(test test.c)