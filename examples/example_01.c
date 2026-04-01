#include "opencstl.h"

int main() {
    char *containers[] = {
        "vector", "list", "set", "map", "stack", "queue", "priority_queue", "unordered_map", "unordered_set"
    };
    size_t len = sizeof(containers) / sizeof(containers[0]);
    printf("Number of elements: %d\n", len);

    // MAP Example
    MAP(int) d = new_map(int, char*, NULL);
    for (int i = 0; i < len; i++) {
        insert(d, i, containers[i]);
    }
    for (int *it = rbegin(d); it != rend(d); it = prev(it)) {
        printf("%d: %s\n", first(it), second(it, char*));
    }
    puts("");
    destroy(d);
}
