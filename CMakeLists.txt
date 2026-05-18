cmake_minimum_required(VERSION 3.10)


project(OpenCSTL C)

set(CMAKE_C_STANDARD 11)
#set(CMAKE_CXX_STANDARD 17)


set(EXE_NAME OpenCSTL)
add_executable(${EXE_NAME} main.c)

if (MSVC)
    target_compile_options(${EXE_NAME} PRIVATE /W3 /utf-8)
else ()
    # CMAKE_C_STANDARD 90 + GCC extensions (gnu90) so `//` line comments,
    # `long long`, variadic macros, and other widely-supported extensions stay
    # available. -Wpedantic would error on these under strict ISO C90.
    #set_target_properties(${EXE_NAME} PROPERTIES C_EXTENSIONS ON)
    target_compile_options(${EXE_NAME} PRIVATE -Wall -Wextra)
endif ()


add_executable(test_glob test_gmp.c)
if (MSVC)
    target_compile_options(test_glob PRIVATE /utf-8)

endif ()

add_executable(bench_sort bench_sort.c)

add_executable(test_fstream test_fstream.c)
#######################################################
target_compile_options(test_fstream PRIVATE
        -O0
        -Wall
        -g
        -fsanitize=address,undefined,leak
        -Werror=incompatible-pointer-types
)

target_compile_definitions(test_fstream PRIVATE NDEBUG)

target_link_options(test_fstream PRIVATE
        -fsanitize=address,undefined,leak
)
###############################################################
add_executable(test_logging test_logging.c)
add_executable(test_string test_string.c)
add_executable(test_gmp test_gmp.c)
add_executable(cenv cenv.c)
#if (MSVC)
#    target_compile_options(test_gmp PRIVATE /W3 /utf-8 /std:c11)
#else ()
#    target_compile_options(test_gmp PRIVATE -Wall -Wextra -Wpedantic)
#endif ()
if (MSVC)
    # _Generic (C11) + UTF-8 한글 주석 처리.
    target_compile_options(bench_sort PRIVATE /utf-8 /std:c11)

    # MSVC: /O2 + 전체 프로그램 최적화 + AVX2 + fast math
    # Debug 빌드는 /RTC1 (런타임 체크) 가 자동 추가되어 /O2 와 충돌하므로
    # 최적화 플래그는 non-Debug 구성에서만 적용.
    target_compile_options(bench_sort PRIVATE
            $<$<NOT:$<CONFIG:Debug>>:/O2>          # 최고 속도 최적화
            $<$<NOT:$<CONFIG:Debug>>:/Oi>          # intrinsic 사용
            $<$<NOT:$<CONFIG:Debug>>:/Ot>          # 속도 우선 (크기 아님)
            $<$<NOT:$<CONFIG:Debug>>:/Oy>          # frame pointer 생략
            $<$<NOT:$<CONFIG:Debug>>:/GL>          # Whole Program Optimization
            $<$<NOT:$<CONFIG:Debug>>:/Gw>          # 전역 변수 최적화
            $<$<NOT:$<CONFIG:Debug>>:/fp:fast>     # 부동소수점 빠르게
            $<$<NOT:$<CONFIG:Debug>>:/arch:AVX512> # AVX 명령어 허용
            $<$<NOT:$<CONFIG:Debug>>:/DNDEBUG>
    )
    # /GL 사용 시 링커에도 /LTCG 필요
    target_link_options(bench_sort PRIVATE
            $<$<NOT:$<CONFIG:Debug>>:/LTCG>
    )

elseif (CMAKE_C_COMPILER_ID MATCHES "Clang")
    # Clang (Linux, macOS, Windows clang-cl 제외)
    target_compile_options(bench_sort PRIVATE
            -O3
            #-march=native       # 현재 CPU 전용 (배포용이 아니면 OK)
            -flto=thin          # ThinLTO (Clang이 GCC보다 빠름)
            -funroll-loops
            -fomit-frame-pointer
            -DNDEBUG
    )
    target_link_options(bench_sort PRIVATE -flto=thin)

elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    # GCC / MinGW-w64
    target_compile_options(bench_sort PRIVATE
            -O3
            #-march=native       # 로컬 CPU에 맞춤
            #-mtune=native
            -flto               # Link-Time Optimization
            -funroll-loops      # 루프 언롤링
            -fno-plt            # PLT 간접 호출 제거 (Linux에서 유의미)
            -fno-semantic-interposition
            -fomit-frame-pointer
            -DNDEBUG
    )
    target_link_options(bench_sort PRIVATE -flto)

    # MinGW에서 종종 필요한 링커 옵션
    if (MINGW)
        target_link_options(bench_sort PRIVATE
                -static-libgcc
                -Wl,--gc-sections   # 미사용 섹션 제거
        )
    endif ()
endif ()


# ------------------------------------------------------------------
# Unit tests (Unity, vendored under tests/unity/)
# ------------------------------------------------------------------
enable_testing()

add_executable(test_deque_algorithm
        tests/test_deque_algorithm.c
        tests/unity/unity.c
)
target_include_directories(test_deque_algorithm PRIVATE ${CMAKE_SOURCE_DIR} tests/unity)
if (MSVC)
    target_compile_options(test_deque_algorithm PRIVATE /utf-8)
endif ()
add_test(NAME test_deque_algorithm COMMAND test_deque_algorithm)