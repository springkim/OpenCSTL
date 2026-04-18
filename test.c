#define OPENCSTL_TRACER
#include "opencstl.h"


int main() {
    char *str = mt19937.uuid();

    char *s = string.substr(str, 2, 8);

    puts(s);

    int n;
    char **datas = string.split(str, "-", &n);
    for (int i = 0; i < n; i++) {
        puts(datas[i]);
    }

    char *new_str = string.replace(str, "-", "==");
    puts(new_str);

    free(str);
    free(new_str);
    free(datas);
    free(s);
    return 0;
}
