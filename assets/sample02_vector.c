#include "opencstl.h"

int main() {
    VECTOR(char*) v = new_vector(char*);
    FILE *fp = NULL;
    FOPEN(&fp, "words_random.txt", "rt");
    char line[1024] = {0};
    while (FGETLINE(fp, line, 1024)) {
        int length = strlen(line);
        char *word = (char *) calloc(length + 1, sizeof(char));
        strncpy(word, line, length);
        push_back(v, word);
    }
    FCLOSE(fp);
    // merge sort
    stable_sort(v,size(v), sizeof(char *), LESS(char*));
    for (int i = 0; i < size(v); i++) {
        puts(v[i]);
    }
    // bsearch example
    char *key = "spring";
    void *iter = bsearch(&key, v,size(v), sizeof(char *), LESS(char*));
    int idx = (char **) iter - (char **) v;
    printf("bsearch: %s(%d)\n", *(char **) iter, idx);
    for (int i = 0; i < size(v); i++) {
        free(v[i]);
    }
    destroy(v);
    return 0;
}