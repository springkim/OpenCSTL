#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"

int main() {


    char **files = glob("../*.c");
    for (char **p = files; *p; p++) {
        printf("%s\n", *p);
    }
    glob_free(files);

    char **all_c = glob("../opencstl/*.h", true);
    for (char **p = all_c; *p; p++) {
        printf("%s\n", *p);
    }
    glob_free(all_c);;
}
