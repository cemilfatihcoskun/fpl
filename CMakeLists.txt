cmake_minimum_required(VERSION 4.4.4)
project(fpl LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

enable_testing()


add_library(fpl_core
    src/fpl.c
    src/vm.c
    src/lexer.c
    src/parser.c
)
target_include_directories(fpl_core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(fpl src/main.c)
target_link_libraries(fpl PRIVATE fpl_core)

set(TEST_SUITES
    test_lexer
    test_vm
)

foreach(test_name ${TEST_SUITES})
    add_executable(${test_name} tests/${test_name}.c)
    target_link_libraries(${test_name} PRIVATE fpl_core)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()


