#include "opencstl.h"

int main() {
    VECTOR(char*) v = new_vector(char*);
    FILE *fp = fstream.open("words_random.txt", "rt");

    char *line = NULL;
    while ((line = fstream.getline(fp)) != NULL) {
        push_back(v, line);
    }
    fstream.close(fp);
    // stable sort (merge sort or tim sort)
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
