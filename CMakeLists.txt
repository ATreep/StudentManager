cmake_minimum_required(VERSION 3.24)
project(StudentManager C)

set(CMAKE_C_STANDARD 23)

add_executable(StudentManager main.c utils.c utils.h lang.c lang.h)
