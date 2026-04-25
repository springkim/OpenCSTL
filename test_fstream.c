//
// Created by spring on 4/21/2026.
//
#include "opencstl/opencstl.h"

int main() {
    FILE *fp = file.open("../test.txt", "r");
    char *buf = file.read(fp);

    puts(buf);
    char *buf2 = string.reverse(buf);
    puts(buf2);

    FILE *fp2 = file.write(fp, buf2);
    file.close(fp2);
    return 0;
}
