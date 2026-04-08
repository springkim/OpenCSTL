#include"opencstl.h"

int main() {
    QUEUE(int) q = new_priority_queue(int);
    for (int i = 0; i < 10; i++) {
        push(q, i);
    }
    while (!empty(q)) {
        printf("[%3d]", top(q));
        pop(q);
    }
    puts("");
    destroy(q);
    return 0;
}
