cmake_minimum_required(VERSION 3.17)
project(listener C)

set(CMAKE_C_STANDARD 11)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_executable(listener listener.c)
