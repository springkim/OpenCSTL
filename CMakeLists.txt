cmake_minimum_required(VERSION 3.10)


project(OpenCSTL C)

set(CMAKE_C_STANDARD 11)

set(EXE_NAME OpenCSTL)
add_executable(${EXE_NAME} main.c)

if (MSVC)
    target_compile_options(${EXE_NAME} PRIVATE /W4 /utf-8)

else ()
    target_compile_options(${EXE_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif ()


add_executable(test test.c)
if (MSVC)
    target_compile_options(test PRIVATE /utf-8)
endif ()