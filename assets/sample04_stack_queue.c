#include "opencstl.h"

int main() {
    STACK(int) s = new_stack(int);
    QUEUE(int) q = new_queue(int);
    for (int i = 0; i < 10; i++) {
        push(s, i);
        push(q, i);
    }
    for (int i = 0; i < 10; i++) {
        printf("%d\n", top(s));
        pop(s);
    }
    for (int i = 0; i < 10; i++) {
        printf("%d\n", front(q));
        pop(q);
    }
    destroy(s);
    destroy(q);
}