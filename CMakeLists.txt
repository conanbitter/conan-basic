cmake_minimum_required(VERSION 3.16)

project(cbasic VERSION 0.1 LANGUAGES C)

set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})

add_executable(cbasic
    src/main.c
)