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
#elif defined(__TINYC__)
#define OPENCSTL_CC "TCC"
#elif defined(_MSC_VER)
#define OPENCSTL_CC "MSVC"
#elif defined(__GNUC__)
#define OPENCSTL_CC "GCC"

#endif


#if defined(__STDC_VERSION__)
#   if __STDC_VERSION__ >= 202311L
#define OPENCSTL_CV "C23"
#   elif __STDC_VERSION__ >= 201710L
#define OPENCSTL_CV "C17 (C18)"
#   elif __STDC_VERSION__ >= 201112L
#define OPENCSTL_CV "C11"
#   elif __STDC_VERSION__ >= 199901L
#define OPENCSTL_CV "C99"
#   else
#define OPENCSTL_CV "C94 (AMD1)"
#   endif
#elif defined(__STDC__)
#define OPENCSTL_CV "C89 (C90)"
#else
#define OPENCSTL_CV "pre-ANSI C (K&R)"
#endif

void print_c_version(void) {
#if defined(__STDC_VERSION__)
    printf("__STDC_VERSION__ : %ldL\n", (long) __STDC_VERSION__);
#endif
}

int main() {
    puts(OPENCSTL_CV);
    puts(OPENCSTL_OS);
    puts(OPENCSTL_CC);
    print_c_version();
    return 0;
}
