cmake_minimum_required(VERSION 4.1)
project(bench_sort C)

set(CMAKE_C_STANDARD 11)

add_executable(bench_sort main.c)

if(MSVC)
    target_compile_options(bench_sort PRIVATE /O2 /Oi /Ot /GL /fp:fast /DNDEBUG)
    target_link_options(bench_sort PRIVATE /LTCG)
elseif(CMAKE_C_COMPILER_ID MATCHES "TinyCC" OR CMAKE_C_COMPILER MATCHES "tcc")
    target_compile_options(bench_sort PRIVATE -DNDEBUG)
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(bench_sort PRIVATE -O3 -march=native -flto -ffast-math -DNDEBUG)
    target_link_options(bench_sort PRIVATE -flto)
endif()