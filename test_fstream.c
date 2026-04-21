//
// Created by spring on 4/21/2026.
//
#include "opencstl/opencstl.h"

int main() {
    FILE *fp = fstream.open("../test.txt", "r");
    char *buf = fstream.read(fp);

    puts(buf);
    char *buf2 = string.reverse(buf);
    puts(buf2);

    FILE *fp2 = fstream.write(fp, buf2);
    fstream.close(fp2);
    return 0;
}
