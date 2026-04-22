#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"


void cstl_vector_test01(void) {
    MsgBoxCLI("OpenCSTL / VECTOR TEST01");

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

    for (int *it = begin(arr); it != end(arr); it = next(it)) {
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

void cstl_vector_test2(void) {
    MsgBoxCLI("OpenCSTL / VECTOR TEST02");
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

void cstl_vector_test3(void) {
    MsgBoxCLI("OpenCSTL / VECTOR TEST03");
    VECTOR(int) vec = new_vector(int);
    const size_type N = 100;
    for (int i = 0; i < N; i++) {
        push_back(vec, mt19937.randint(-N, N));
    }

    pmsort(vec, size(vec), sizeof(int), LESS(int));
    for (int i = 0; i < size(vec); i++) {
        printf("[%d]", vec[i]);
        if (i < size(vec) - 1) {
            putchar(',');
        } else {
            putchar('\n');
        }
    }


    destroy(vec);
}


void cstl_list_test01(void) {
    MsgBoxCLI("OpenCSTL / LIST TEST01");
    LIST(int) list = new_list(int);

    for (int i = 0; i < 10; i++) {
        push_back(list, i);
    }
    ///[0] [1] [2] [3] [4] [5] [6] [7] [8] [9]
    printf("list size: %d\n", size(list));

    pop_back(list);
    ///[0] [1] [2] [3] [4] [5] [6] [7] [8]
    insert(list, begin(list), 777);
    ///[777] [0] [1] [2] [3] [4] [5] [6] [7] [8]
    insert(list, find(list, 4), 5, 999);
    ///[777] [0] [1] [2] [3] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
    erase(list, find(list, 2), find(list, 999));
    ///[777] [0] [1] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
    for (int *it = begin(list); it != end(list); it = next(it)) {
        printf("[%d] ", *it);
    }
    puts("");
    printf("list size: %d\n", size(list));
    printf("front : %d\n", front(list));
    printf("back : %d\n", back(list));

    resize(list, 20);
    printf("list size: %d\n", size(list));
    resize(list, 0);
    printf("list size: %d\n", size(list));

    for (int i = 0; i < 50; i++) {
        int val = mt19937.randint(0, 1000);
        push_back(list, val);
    }


    sort(list);

    logging.info("list size: %d", size(list));
    for (int *it = begin(list); it != end(list); it = next(it)) {
        printf("[%d]->", *it);
    }
    puts("[NULL]");


    destroy(list);
}


void cstl_set_test01(void) {
    MsgBoxCLI("OpenCSTL / SET TEST01");
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

void cstl_map_test01(void) {
    MsgBoxCLI("OpenCSTL / MAP TEST01");
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

void cstl_deque_test01(void) {
    MsgBoxCLI("OpenCSTL / DEQUE TEST01");
    DEQUE(int) deque = new_deque(int);
    for (int i = 0; i < 10; i++) {
        push_back(deque, i);
    }
    printf("deque pb size: %d\n", size(deque));
    for (int i = 0; i < 10; i++) {
        push_front(deque, i);
    }
    printf("deque pf size: %d\n", size(deque));
    // for (int i = 0; i < size(deque); i++) {
    //     printf("[%3d]", deque[i]);
    // }
    for (int *it = begin(deque); it != end(deque); it = next(it)) {
        printf("[%d]", *it);
    }
    puts("");

    printf("front : %d\n", front(deque));
    printf("back : %d\n", back(deque));


    assign(deque, 5);
    ///[0] [0] [0] [0] [0]
    printf("size: %d\n", size(deque));
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

void cstl_stack_test01(void) {
    MsgBoxCLI("OpenCSTL / STACK TEST01");
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

void cstl_queue_test01(void) {
    MsgBoxCLI("OpenCSTL / QUEUE TEST01");
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

void cstl_priority_queue_test01(void) {
    MsgBoxCLI("OpenCSTL / PRIORITY_QUEUE TEST01");
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

void cstl_priority_queue_test02(void) {
    MsgBoxCLI("OpenCSTL / PRIORITY_QUEUE TEST02");
    Edge **vec = new_vector(Edge*);
    assign(vec, 7);
    for (int i = 0; i < size(vec); i++) {
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

void cstl_unordered_set_test01(void) {
    MsgBoxCLI("OpenCSTL / UNORDERED_SET TEST01");
    UNORDERED_SET(int) h = new_unordered_set(int);

    for (int i = 1; i < 100; i++) {
        insert(h, i);
    }

    size_t capacity_max = max_capacity(h);

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

void cstl_unordered_set_test02(void) {
    MsgBoxCLI("OpenCSTL / UNORDERED_SET TEST02");
    UNORDERED_SET(int) v = new_unordered_set(int);
    watch t_beg = chrono.now();
    for (int i = 0; i < 5000000; ++i) {
        insert(v, mt19937.randint(0,10000000));
    }
    watch t_end = chrono.now();
    printf("size: %d\n", size(v));
    double ms = chrono.duration(t_beg, t_end);
    printf("duration : %f ms\n", ms);
    destroy(v);
}

void test01(void) {
    cstl_vector_test01();
    cstl_vector_test2();
    cstl_vector_test3();
    cstl_list_test01();

    cstl_set_test01();
    cstl_map_test01();


    cstl_deque_test01();
    cstl_stack_test01();
    cstl_queue_test01();
    cstl_priority_queue_test01();


    cstl_priority_queue_test02();

    cstl_unordered_set_test01();
    cstl_unordered_set_test02();
}


void test03(void) {
    FILE *fp = fstream.open("../words_random.txt", "r");
    VECTOR(char*) words = new_vector(char*);
    char *line = NULL;
    while ((line = fstream.getline(fp)) != NULL) {
        push_back(words, line);
    }
    fstream.close(fp);


    for (int i = 0; i < size(words); i++) {
        logging.info(words[i]);
    }
    for (int i = 0; i < size(words); i++) {
        free(words[i]);
    }
    destroy(words);
}

void test04(void) {
    FILE *fp = fstream.open("../words_random.txt", "r");
    char *file = fstream.read(fp);
    fstream.close(fp);
    printf("%s\n", file);
    free(file);
}


int main(void) {


    logging.info(opencstl_env());
    logging.info(opencstl_version());

    //verify("Error message");

    test01();

    // float a = 0;
    // __auto_type b = 1;
    //auto b = 1;

    return 0;
}
