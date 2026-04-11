#include "opencstl.h"

int main() {
    DEQUE(int) dq = new_deque(int);
    for (int i = 0; i < 10; i++) {
        push_back(dq, i);
    }
    printf("deque size: %d\n", size(dq));
    for (int i = 0; i < 10; i++) {
        push_front(dq, i);
    }
    printf("deque size: %d\n", size(dq));
    for (int i = 0; i < size(dq); i++) {
        printf("[%d]", dq[i]);
    }
    puts("");
    printf("front : %d\n", front(dq));
    printf("back : %lld\n", back(dq));

    assign(dq, 5);
    // [0] [0] [0] [0] [0] 
    printf("size: %d\n", size(dq));
    for (int i = 0; i < size(dq); i++) {
        printf("[%d]", dq[i]);
    }
    puts("");

    assign(dq, 5, 7);
    resize(dq, 10);
    // [7] [7] [7] [7] [7] [0] [0] [0] [0] [0] 
    for (int i = 0; i < size(dq); i++) {
        printf("[%d]", dq[i]);
    }
    puts("");
    destroy(dq);
    return 0;
}
