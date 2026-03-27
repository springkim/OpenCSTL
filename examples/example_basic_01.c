#include "opencstl.h"

void example_vector() {
    VECTOR(int) arr = new_vector(int);

    for (int i = 0; i < 10; i++) {
        push_back(arr, i);
    }
    for (int *it = begin(arr); it != end(arr); it++) {
        printf("[%3d]", *it);
    }
    puts("");
    destroy(arr);
}

void example_list() {
    LIST(int) arr = new_list(int);
    for (int i = 10; i >= 0; i--) {
        push_back(arr, i);
    }
    for (int *it = begin(arr); it != end(arr); it = next(it)) {
        printf("[%3d]", *it);
    }
    puts("");
    destroy(arr);
}

void example_set() {
    SET(int) set = new_set(int, COMPARE(int));
    for (int i = 0; i < 10; i++) {
        insert(set, rand()%100);
    }
    for (int *it = begin(set); it != end(set); it = next(it)) {
        printf("[%3d]", *it);
    }
    puts("");
    destroy(set);
}

void example_map() {
    char *containers[] = {
        "vector", "list", "set", "map", "stack", "queue", "priority_queue", "unordered_map", "unordered_set"
    };
    size_t len = sizeof(containers) / sizeof(containers[0]);
    printf("Number of elements: %d\n", len);
    MAP(int) d = new_map(int, char*, COMPARE(int));
    for (int i = 0; i < len; i++) {
        insert(d, i, containers[i]);
    }
    for (int *it = rbegin(d); it != rend(d); it = prev(it)) {
        printf("[%3d : %s]\n", first(it), second(it, char*));
    }
    puts("");
    destroy(d);
}

int main() {
    example_vector();
    example_list();
    example_set();
    example_map();
    return 0;
}
