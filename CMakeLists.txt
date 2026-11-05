cmake_minimum_required(VERSION 4.4.4)
project(fpl LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

enable_testing()

add_executable(fpl
    src/fpl.c
    src/lexer.c
    src/parser.c
    src/main.c
)

target_include_directories(fpl
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(fpl_tests
    src/fpl.c
    src/lexer.c
    src/parser.c

    tests/test_lexer.c
)

target_include_directories(fpl_tests
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(NAME CoreUnitTests COMMAND fpl_tests)

