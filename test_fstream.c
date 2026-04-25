#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"

int main() {
    FILE *fp = file.open("../test.txt", "r");
    char *buf = file.read(fp);

    puts(buf);
    char *buf2 = string.reverse(buf);
    free(buf);

    puts(buf2);

    FILE *fp2 = file.write(fp, buf2);
    file.close(fp2);

    free(buf2);
    return 0;
}
