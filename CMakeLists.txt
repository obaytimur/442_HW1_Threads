cmake_minimum_required(VERSION 3.22)
project(HW1 C)

set(CMAKE_C_STANDARD 99)

add_executable(HW1 main.c atom.c tube.c)

target_link_libraries(HW1 m)
