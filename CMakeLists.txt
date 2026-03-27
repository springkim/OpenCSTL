cmake_minimum_required(VERSION 3.10)
project(OpenCSTL C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_COMPILER 17)
set(EXE_NAME OpenCSTL)
add_executable(${EXE_NAME} main.c)

add_executable(PERFTEST perftest.c)

add_executable(HASH_COLLECTION opencstl/dev/hash.c)

add_executable(EXAMPLE example.c)