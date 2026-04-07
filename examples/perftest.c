#include <locale.h>
#include "opencstl.h"
#include <stdio.h>
#include <stdlib.h>

int count_lines(const char *filename) {
    FILE *fp = NULL;
    FOPEN(&fp, filename, "r");
    if (!fp) return -1;

    int lines = 0;
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') lines++;
    }
    fclose(fp);
    return lines;
}

int main() {
    setlocale(LC_ALL, "");

    int lines = count_lines("words_random.txt");

    FILE *fp = NULL;

    FOPEN(&fp, "words_random.txt", "rt");


    if (!fp) {
        fprintf(stderr, "Error opening file\n");
    }
    VECTOR(char*) arr = new_vector(char*);

    const int LINE_SIZE = 256;
    char *strings = (char *) calloc(lines, sizeof(char) * LINE_SIZE);
    memset(strings, 0, sizeof(char) * LINE_SIZE);
    watch t_beg = tick();
    int index = 0;
    char *line = strings;
    //int max_size = 0;
    while (GETLINE(fp, line, LINE_SIZE)) {
        size_t len = strlen(line);
        push_back(arr, line);
        line += LINE_SIZE;
        index++;
    }
    //printf("max size: %lld\n", max_size);
    double push_back_ms = lap(t_beg, tick());
    printf("size: %lu\telapsed_time: %lf\n",cstl_size(arr), push_back_ms);


    t_beg = tick();

    //char ***tree = cstl_set(char*, StringCmp);
    UNORDERED_SET(char*) tree = new_unordered_set(char*);
    for (int i = 0; i < size(arr); i++) {
        char *line = arr[i];
        insert(tree, line);
        //puts(line);
    }
    double insert_time = lap(t_beg, tick());
    printf("size: %lu\telapsed_time: %lf\n",size(tree), insert_time);


    // for (char **it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
    //     printf("%s\n",cstl_value(it, char*));
    // }
    free(strings);
    cstl_free(tree);
    cstl_free(arr);
    return 0;
}
