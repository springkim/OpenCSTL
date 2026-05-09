#include "opencstl/opencstl.h"
#include <stdlib.h>
int cmp_int(const void *a, const void *b) {
    int x = *((const int *) a);
    int y = *((const int *) b);
    return (x > y) - (x < y);
}
void example_deque() {
    DEQUE(int) q = new_deque(int);
    for (int i = 0; i < 10; i++) {
        int val = mt19937.randint(0, 1000);
        push_back(q, val);
    }
    insert(q, q + 5, 777);
    insert(q, q + 8, 3, 999);
    for (int i = 0; i < size(q); i++) {
        printf("[%3d]", q[i]);
    }
    puts("");
    qsort(q, size(q), sizeof(int), cmp_int);
    for (int i = 0; i < size(q); i++) {
        printf("[%3d]", q[i]);
    }
    puts("");
    destroy(q);
}
int main() {
    example_deque();
    return 0;
}