#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "opencstl/opencstl.h"
#define DECORATE(STR) {int padding = 70 - strlen(STR);int margin=0;if(padding%2==1)margin=1;else margin=0;for (int i = 0; i < padding/2; i++) { putchar('#');  } printf(STR); for (int i = 0; i < padding/2+margin; i++) { putchar('#'); } putchar('\n');}


void cstl_vector_test() {
    DECORATE("OPENCSTL{vector} test begin");
    VECTOR(int) arr = new_vector(int);

    ///[0] [1] [2] [3] [4] [5] [6] [7] [8] [9]
    for (int i = 0; i < 10; i++) {
        push_back(arr, i);
    }

    ///[0] [1] [2] [3] [4] [5] [6] [7] [8]
    pop_back(arr);


    ///[777] [0] [1] [2] [3] [4] [5] [6] [7] [8]
    insert(arr, arr + 0, 777);

    ///[777] [0] [1] [2] [3] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
    insert(arr, arr + 5, 5, 999);

    ///[777] [0] [1] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
    erase(arr, arr + 3, arr + 5);

    ///[777] [0] [1] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8] [-1] [-1]
    resize(arr, 15, -1);

    for (int *it = begin(arr); it != end(arr); it++) {
        printf("[%d] ", *it);
    }
    puts("");

    int *pos = find(arr, 7);
    if (pos != NULL) {
        printf("find : [%3d]\n", *pos);
    }
    assign(arr, 5);
    ///[0] [0] [0] [0] [0]
    for (int i = 0; i < size(arr); i++) {
        printf("[%d] ", arr[i]);
    }
    puts("");
    assign(arr, 5, 7);
    ///[7] [7] [7] [7] [7]
    for (int i = 0; i < size(arr); i++) {
        printf("[%d] ", arr[i]);
    }
    puts("");
    destroy(arr);
}

void cstl_vector_test2() {
    DECORATE("opencstl{vector 2d} test begin");
    VECTOR(int*) matrix = new_vector(int*);
    const size_t sz = 7;
    assign(matrix, sz);
    for (int i = 0; i < size(matrix); i++) {
        matrix[i] = new_vector(int);
        assign(matrix[i], sz);
    }

    for (int i = 0; i < size(matrix); i++) {
        for (int j = 0; j < size(matrix[i]); j++) {
            matrix[i][j] = i * j;
        }
    }
    for (int i = 0; i < size(matrix); i++) {
        for (int j = 0; j < size(matrix[i]); j++) {
            printf("[%3d]", matrix[i][j]);
        }
        puts("");
    }

    for (int i = 0; i < size(matrix); i++) {
        destroy(matrix[i]);
    }
    destroy(matrix);
}

void cstl_vector_test3() {
    DECORATE("opencstl{vector/qsort} test begin");
    VECTOR(int) vec = new_vector(int);
    for (int i = 0; i < 100; i++) {
        int val = rand32();
        push_back(vec, val);
    }


    stable_sort(vec,size(vec), sizeof(int), LESS(int));
    for (int i = 0; i < size(vec); i++) {
        printf("Sorted: [%lld]\n", vec[i]);
    }


    destroy(vec);
}


void cstl_list_test02() {
    DECORATE("opencstl{list} test begin");
    LIST(int) list = new_list(int);
    for (int i = 0; i < 10; i++) {
        push_back(list, i);
    }
    ///[0] [1] [2] [3] [4] [5] [6] [7] [8] [9]
    printf("list size: %lld\n", size(list));

    pop_back(list);
    ///[0] [1] [2] [3] [4] [5] [6] [7] [8]
    insert(list, cstl_begin(list), 777);
    ///[777] [0] [1] [2] [3] [4] [5] [6] [7] [8]
    insert(list, cstl_find(list, 4), 5, 999);
    ///[777] [0] [1] [2] [3] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
    erase(list, cstl_find(list, 2), cstl_find(list, 999));
    ///[777] [0] [1] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
    for (int *it = begin(list); it != end(list); it = next(it)) {
        printf("[%d] ", *it);
    }
    puts("");
    printf("list size: %lld\n", size(list));
    printf("front : %d\n", front(list));
    printf("back : %d\n", back(list));

    resize(list, 20);
    printf("list size: %lld\n", size(list));
    resize(list, 3);
    printf("list size: %lld\n", size(list));
    destroy(list);
}


void cstl_set_test() {
    DECORATE("OPENCSTL{set} test begin");
    SET(float) tree = new_set(float);
    for (float i = 0; i < 100; i++) {
        insert(tree, i);
    }

    /// [0.0] [1.0] ... [98.0] [99.0]
    for (float i = 50; i < 70; i++) {
        float *it = find(tree, i);
        erase(tree, it);
    }

    /// [0] [1] ... [48] [49] [70] [71] ... [98] [99]
    for (float *it = begin(tree); it != end(tree); it = next(it)) {
        printf("[%f]", *it);
    }
    puts("");
    printf("size : %d\n", size(tree));
    destroy(tree);
}

void cstl_map_test() {
    DECORATE("OPENCSTL{map} test begin");
    MAP(int) tree = new_map(int, double);
    for (int i = 0; i < 10; i++) {
        double d = (double) i * i;
        insert(tree, i, d);
    }
    ///[0]{0} [1]{1} [2]{4} [3]{9} [4]{16} [5]{25} [6]{36} [7]{49} [8]{64} [9]{81}
    for (int i = 0; i < 10; i += 2) {
        int *it = find(tree, i);
        erase(tree, it);
    }
    ///[1]{1} [3]{9} [5]{25} [7]{49} [9]{81}
    for (int *it = begin(tree); it != end(tree); it = next(it)) {
        printf("[%1d]{%.1f} ", first(it), second(it, double));
    }
    puts("");
    printf("size : %d\n", size(tree));
    destroy(tree);
}

void cstl_deque_test() {
    DECORATE("OPENCSTL{deque} test begin");
    DEQUE(int) deque = new_deque(int);
    for (int i = 0; i < 10; i++) {
        push_back(deque, i);
    }
    printf("deque pb size: %lld\n", size(deque));
    for (int i = 0; i < 10; i++) {
        push_front(deque, i);
    }
    printf("deque pf size: %lld\n", size(deque));
    for (int i = 0; i < size(deque); i++) {
        printf("[%3d]", deque[i]);
    }
    puts("");

    printf("front : %d\n", deque[0]);
    printf("front : %d\n", front(deque));
    printf("back : %lld\n", back(deque));


    assign(deque, 5);
    ///[0] [0] [0] [0] [0]
    printf("size: %lld\n", size(deque));
    for (int i = 0; i < size(deque); i++) {
        printf("[%d] ", deque[i]);
    }
    puts("");
    assign(deque, 5, 7);
    resize(deque, 10);
    ///[7] [7] [7] [7] [7]
    for (int i = 0; i < size(deque); i++) {
        printf("[%d] ", deque[i]);
    }
    puts("");


    destroy(deque);
}

void cstl_stack_test() {
    DECORATE("OPENCSTL{stack} test begin");
    STACK(int) stack = new_stack(int);
    for (int i = 0; i < 10; i++) {
        push(stack, i);
    }
    for (int i = 0; i < 10; i++) {
        printf("[%3d]", top(stack));
        pop(stack);
    }
    puts("");
    destroy(stack);
}

void cstl_queue_test() {
    DECORATE("OPENCSTL{queue} test begin");
    QUEUE(int) queue = new_queue(int);
    for (int i = 0; i < 100; i++) {
        push(queue, i);
    }
    for (int i = 0; i < 100; i++) {
        printf("[%3d]", front(queue));
        pop(queue);
    }
    puts("");
    destroy(queue);
}

void cstl_priority_queue_test() {
    DECORATE("OPENCSTL{priority_queue} test begin");
    QUEUE(int) queue = new_priority_queue(int);
    for (int i = 0; i < 10; i++) {
        push(queue, i);
    }
    while (!empty(queue)) {
        printf("[%3d]", top(queue));
        pop(queue);
    }
    puts("");
    destroy(queue);
}

typedef struct {
    int cost, to;
} Edge;

Edge make_edge(int cost, int to) {
    Edge r = {cost, to};
    return r;
}

int compare_edge(const void *_a, const void *_b) {
    Edge *a = (Edge *) _a;
    Edge *b = (Edge *) _b;
    return a->cost > b->cost ? -1 : a->cost < b->cost;
}

int *dijkstra(Edge **vec, int beg) {
    VECTOR(int) d = new_vector(int);
    QUEUE(Edge) pqueue = new_priority_queue(Edge, compare_edge);
    assign(d, size(vec), 99999);
    d[beg] = 0;
    push(pqueue, make_edge(0, beg));
    while (!empty(pqueue)) {
        Edge e = top(pqueue);
        pop(pqueue);
        int end = size(vec[e.to]);
        for (int i = 0; i < end; i++) {
            int *a = &d[vec[e.to][i].to];
            int b = d[e.to];
            int c = vec[e.to][i].cost;
            if (*a > b + c) {
                *a = b + c;
                int value = vec[e.to][i].to;
                push(pqueue, make_edge(*a, value));
            }
        }
    }
    destroy(pqueue);
    return d;
}

void cstl_priority_queue_test2() {
    DECORATE("OPENCSTL{priority_queue/dijkstra} test begin");
    Edge **vec = new_vector(Edge*);
    cstl_assign(vec, 7);
    for (int i = 0; i < cstl_size(vec); i++) {
        vec[i] = new_vector(Edge);
    }
    push_back(vec[1], make_edge(10, 2));
    push_back(vec[1], make_edge(30, 3));
    push_back(vec[1], make_edge(15, 4));
    push_back(vec[2], make_edge(20,5));
    push_back(vec[3], make_edge(5,6));
    push_back(vec[4], make_edge(5,3));
    push_back(vec[4], make_edge(20,6));
    push_back(vec[5], make_edge(20, 6));
    push_back(vec[6], make_edge(20, 4));

    int *d = dijkstra(vec, 1);
    for (int i = 0; i < size(d); i++) {
        printf("%d\n", d[i]);
    }
    destroy(d);
    for (int i = 0; i < size(vec); i++) {
        destroy(vec[i]);
    }
    destroy(vec);
}

void cstl_hash_test() {
    DECORATE("OPENCSTL{unordered_set} test begin");
    UNORDERED_SET(int) h = new_unordered_set(int);

    for (int i = 1; i < 100; i++) {
        insert(h, i);
    }

    size_t capacity_max = capacity(h);
    for (int i = 0; i < (int) capacity_max; i++) {
        int k = h[i];
        if (k == 0) {
            printf("[---]");
        } else {
            printf("[%3d]", k);
        }
    }

    printf("\n=============================================\n");
    for (int *it = rbegin(h); it != rend(h); it = (int *) prev(it)) {
        printf("[%3d]", *it);
    }
    printf("\n");
    destroy(h);
}

void test01() {
    cstl_vector_test();
    cstl_vector_test2();
    cstl_list_test02();
    cstl_set_test();
    cstl_map_test();
    cstl_deque_test();
    cstl_stack_test();
    cstl_queue_test();
    cstl_priority_queue_test();
    cstl_priority_queue_test2();
    cstl_vector_test3();
    cstl_hash_test();
}

void test02() {
    VECTOR(int) v = new_vector(int);
    for (int i = 0; i < 1000; i++) {
        push_back(v, rand32()%10000);
    }

    sort(v,size(v), sizeof(int),GREATER(int));


    for (int i = 0; i < size(v); i++) {
        printf("%d\n", v[i]);
    }

    exit(0);
}

#include <stdio.h>

const char *get_c_version(void) {
#if defined(__STDC_VERSION__)
#   if __STDC_VERSION__ >= 202311L
    return "C23";
#   elif __STDC_VERSION__ >= 201710L
    return "C17 (C18)";
#   elif __STDC_VERSION__ >= 201112L
    return "C11";
#   elif __STDC_VERSION__ >= 199901L
    return "C99";
#   else
    return "C94 (AMD1)";
#   endif
#elif defined(__STDC__)
    return "C89 (C90)";
#else
    return "pre-ANSI C (K&R)";
#endif
}

void print_c_version(void) {
    printf("C Standard : %s\n", get_c_version());

#if defined(__STDC_VERSION__)
    printf("__STDC_VERSION__ : %ldL\n", (long) __STDC_VERSION__);
#endif

    // compiler
#if defined(_MSC_VER)
    printf("Compiler   : MSVC %d\n", _MSC_VER);
#elif defined(__clang__)
    printf("Compiler   : Clang %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__GNUC__)
    printf("Compiler   : GCC %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#else
    printf("Compiler   : Unknown\n");
#endif

    // platform
#if defined(_WIN32)
    printf("Platform   : Windows\n");
#elif defined(__APPLE__)
    printf("Platform   : macOS\n");
#elif defined(__linux__)
    printf("Platform   : Linux\n");
#else
    printf("Platform   : Unknown\n");
#endif
}


int main() {
    //test02();
    //cstl_set_test();
    //print_c_version();
     test01();
    //cstl_priority_queue_test();
    return 0;
}
