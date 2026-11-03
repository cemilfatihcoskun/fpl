cmake_minimum_required(VERSION 4.4.4)
project(fpl LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(fpl
    src/main.c
)

target_include_directories(fpl
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

