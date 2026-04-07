//
// Created by spring on 3/24/2026.
//
#include "opencstl/opencstl.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>


int main() {
    setlocale(LC_ALL, "");
    FILE *fp = NULL;
#if defined(_WIN32) || defined(_WIN64) ||defined(__TINYC__)
    fp = fopen("C:/Users/spring/Documents/GitHub/OpenCSTL/words.txt", "rt");
#elif defined(__linux__) && defined(__GNUC__)
    fp = fopen("/mnt/c/Users/spring/Documents/GitHub/OpenCSTL/words.txt", "rt");
    if (!fp) {
        fp = fopen("/home/spring/Documents/Github/OpenCSTL/words.txt", "rt");
    }
#elif defined(__APPLE__)
    fp = fopen("/Users/spring/Documents/GitHub/OpenCSTL/words.txt", "rt");
#else
    //fopen_s(&fp, "C:/Users/spring/Documents/GitHub/OpenCSTL/all_queries.txt", "rt");
    fopen_s(&fp, "C:/Users/spring/Documents/GitHub/OpenCSTL/words1.txt", "rt");
#endif
    if (!fp) {
        printf("Error opening file\n");
    }
    char **arr = cstl_vector(char*);
    //int lines = 26410825;
    int lines = 370106;
    const int LINE_SIZE = 256;
    char *strings = (char *) calloc(lines, sizeof(char) * LINE_SIZE);
    memset(strings, 0, sizeof(char) * LINE_SIZE);
    watch t_beg = tick();
    int index = 0;
    char *line = strings;
    //int max_size = 0;
    while (fgets(line, LINE_SIZE, fp)) {
        size_t len = strlen(line);

        // 마지막 문자가 개행문자인지 확인 후 제거
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // 윈도우 스타일(\r\n) 대응이 필요한 경우
        if (len > 1 && line[len - 2] == '\r') {
            line[len - 2] = '\0';
        }
        cstl_push_back(arr, line);
        //puts(line);
        line += LINE_SIZE;
        index++;
        // if (index == 500) {
        //     break;
        // }
    }
    //printf("max size: %lld\n", max_size);
    double push_back_ms = lap(t_beg, tick());
    printf("size: %lu\telapsed_time: %lf\n",cstl_size(arr), push_back_ms);


    t_beg = tick();

    //char ***tree = cstl_set(char*, StringCmp);
    UNORDERED_SET(char*) tree = new_unordered_set(char*);
    for (int i = 0; i < cstl_size(arr); i++) {
        char *line = arr[i];
        cstl_insert(tree, line);
        //puts(line);
    }
    double insert_time = lap(t_beg, tick());
    printf("size: %lu\telapsed_time: %lf\n",cstl_size(tree), insert_time);


    // for (char **it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
    //     printf("%s\n",cstl_value(it, char*));
    // }
    free(strings);
    cstl_free(tree);
    cstl_free(arr);
    return 0;
}
