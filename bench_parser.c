#include <stdio.h>
#include <locale.h>
#include "opencstl/opencstl.h"

int main() {
    setlocale(LC_ALL, "");
    FILE *fp;
    fp = fstream.open("../settings.json", "rt");

    // char* line = NULL;
    // while ((line=fstream.getline(fp))!=NULL){
    //     puts(line);
    // }
    size_t n;
    char *buffer = fstream.read(fp);

    printf("%s", buffer);
}
