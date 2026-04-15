#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"

// typedef int (*putchar_fn)(int c);
//
// #define PRINT(...) _PRINT(putchar,__VA_ARGS__)
// #define _PRINT(PC,_1,_2,_3,_4,_5,_6,_7,_8,N,...) _print ## _ ## N
//
// void _print_0(putchar_fn pc, ...) {
// }

int main() {
    // SetConsoleOutputCP(65001); // stdout을 UTF-8로
    // SetConsoleCP(65001); // stdin도 UTF-8로

    char *str = mt19937.uuid();

    char *s = string.substr(str, 2, 8);

    puts(s);

    int n;
    char **datas = string.split(str, '-', &n);
    for (int i = 0; i < n; i++) {
        puts(datas[i]);
    }

    char *new_str = string.replace(str, "-", "/");
    puts(new_str);

    free(new_str);
    free(datas);
    free(s);
    return 0;
}
