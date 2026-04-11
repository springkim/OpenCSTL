#include "opencstl.h"

int main() {
    LIST(int) lst = new_list(int);
    for (int i = 0; i < 10; i++) {
        int val = 10 - i;
        push_back(lst, val);
    }
    for (int *it = begin(lst); it != end(lst); it = next(it)) {
        printf("%d\n", *it);
    }
    destroy(lst);
    return 0;
}