#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"

int main() {
    char *s = "aabxaabxaa";
    char *p = "a";
    int count = 0;
    int *pos = string.kmp(s, p, &count);

    string.reverse(s);
    for (int i = 0; i < count; i++) {
        printf("%d\n", pos[i]);
    }
    free(pos);


}
