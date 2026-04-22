cmake_minimum_required(VERSION 3.10)


project(OpenCSTL C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)


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

add_executable(bench_sort bench_sort.c)
add_executable(bench_parser bench_parser.c
        test_curl.c)
add_executable(test_fstream test_fstream.c)
add_executable(test_logging test_logging.c)
add_executable(test_string test_string.c)
add_executable(test_json test_json.c)
add_executable(test_hashtable test_hashtable.c)
add_executable(test_curl test_curl.c)
#if(MSVC)
#    # MSVC: /O2 + 전체 프로그램 최적화 + AVX2 + fast math
#    target_compile_options(bench_sort PRIVATE
#            /O2         # 최고 속도 최적화
#            /Oi         # intrinsic 사용
#            /Ot         # 속도 우선 (크기 아님)
#            /Oy         # frame pointer 생략
#            /GL         # Whole Program Optimization (LTO 상응)
#            /Gw         # 전역 변수 최적화
#            /fp:fast    # 부동소수점 빠르게 (정렬엔 영향 없지만 관례)
#            /arch:AVX512  # AVX2 명령어 허용 (최신 CPU 기준)
#            /DNDEBUG
#    )
#    # /GL 사용 시 링커에도 /LTCG 필요
#    target_link_options(bench_sort PRIVATE /LTCG)
#
#elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
#    # Clang (Linux, macOS, Windows clang-cl 제외)
#    target_compile_options(bench_sort PRIVATE
#            -O3
#            -march=native       # 현재 CPU 전용 (배포용이 아니면 OK)
#            -flto=thin          # ThinLTO (Clang이 GCC보다 빠름)
#            -funroll-loops
#            -fomit-frame-pointer
#            -DNDEBUG
#    )
#    target_link_options(bench_sort PRIVATE -flto=thin)
#
#elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
#    # GCC / MinGW-w64
#    target_compile_options(bench_sort PRIVATE
#            -O3
#            -march=native       # 로컬 CPU에 맞춤
#            -mtune=native
#            -flto               # Link-Time Optimization
#            -funroll-loops      # 루프 언롤링
#            -fno-plt            # PLT 간접 호출 제거 (Linux에서 유의미)
#            -fno-semantic-interposition
#            -fomit-frame-pointer
#            -DNDEBUG
#    )
#    target_link_options(bench_sort PRIVATE -flto)
#
#    # MinGW에서 종종 필요한 링커 옵션
#    if(MINGW)
#        target_link_options(bench_sort PRIVATE
#                -static-libgcc
#                -Wl,--gc-sections   # 미사용 섹션 제거
#        )
#    endif()
#endif()