#define OPENCSTL_TRACER
#include <locale.h>
#include "opencstl/opencstl.h"

int main() {
    setlocale(LC_ALL, ".UTF-8");
    SetConsoleOutputCP(65001);
    // fs.makedirs("./test/folder");
    // FILE *fp = fstream.open("./test/folder/a.txt", "wt");
    // for (int i = 0; i < 10; i++) {
    //     fprintf(fp, "%d\n", i);
    // }
    // fstream.close(fp);
    //
    // if (fs.exists("./test/folder/a.txt")) {
    //     puts("Exists!!");
    // }
    // char *basename = fs.basename("./test/folder/a.txt");
    // puts(basename);
    // free(basename);


    char **files = glob("../*.c");
    for (char **p = files; *p; p++) {
        printf("%s\n", *p);
    }
    glob_free(files);

    char **all_c = glob("../opencstl/*.h", true);
    for (char **p = all_c; *p; p++) {
        printf("%s\n", *p);
    }
    glob_free(all_c);

    wprintf(L"%s\n", L"■■■■");
}
