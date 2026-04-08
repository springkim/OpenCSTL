//
// Created by spring on 4/8/2026.
//
#include <stdio.h>
#if defined(_WIN32) || defined(_WIN64)
#define OPENCSTL_OS "Windows"
#elif defined(__linux__)
#define OPENCSTL_OS "Linux"
#elif defined(__APPLE__)
#define OPENCSTL_OS "MacOS"
#endif

#if defined(__clang__)
#define OPENCSTL_CC "Clang"
#elif defined(_MSC_VER)
#define OPENCSTL_CC "MSVC"
#elif defined(__GNUC__)
#define OPENCSTL_CC "GCC"
#elif defined(__TINYC__)
#define OPENCSTL_CC "TCC"
#endif

int main() {
    puts(OPENCSTL_OS);
    puts(OPENCSTL_CC);
}