#include <stdio.h>
int main() {
#ifdef __LCC__
    printf("__LCC__ = %d\n", __LCC__);
#endif
#ifdef __LCCWIN__
    printf("__LCCWIN__\n");
#endif
#ifdef __LCCOPTIMLEVEL
    printf("__LCCOPTIMLEVEL = %d\n", __LCCOPTIMLEVEL);
#endif
#ifdef _WIN64
    printf("_WIN64\n");
#endif
    return 0;
}
