cmake_minimum_required(VERSION 3.17)
project(RFT C)

set(CMAKE_C_STANDARD 11)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # lots of warnings - you should fix them
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_executable(server server.c)
add_executable(client client.c)
