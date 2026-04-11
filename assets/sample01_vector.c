#include"opencstl.h"

int main() {
    VECTOR(int) v = new_vector(int);
    for (int i = 100; i > 0; i--) {
        push_back(v, i);
    }
    qsort(v, size(v), sizeof(int), LESS(int));
    for (int *it = begin(v); it != end(v); it++) {
        printf("%d\n", *it);
    }
    destroy(v);
    return 0;
}
