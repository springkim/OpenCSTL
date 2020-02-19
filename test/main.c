#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../opencstl/opencstl.h"
#define Decorate(STR) for (int i = 0; i < 30; i++) { putchar('='); } printf(STR); for (int i = 0; i < 30; i++) { putchar('='); } putchar('\n');
void cstl_vector_test() {
    Decorate("opencstl{vector} test begin");
    int *arr = cstl_vector(int);

    for (int i = 0; i < 10; i++) {
        cstl_push_back(arr, i);
    }

    ///[0] [1] [2] [3] [4] [5] [6] [7] [8] [9]
    cstl_pop_back(arr);
    ///[0] [1] [2] [3] [4] [5] [6] [7] [8]
    cstl_insert(arr, arr + 0, 777);
    ///[777] [0] [1] [2] [3] [4] [5] [6] [7] [8]
    cstl_insert(arr, arr + 5, 5, 999);
    ///[777] [0] [1] [2] [3] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
    cstl_erase(arr, arr + 3, arr + 5);

    ///[777] [0] [1] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
    cstl_resize(arr, 15, -1);

    ///[777] [0] [1] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8] [-1] [-1]
    for (int *it = cstl_begin(arr); it != cstl_end(arr); it++) {
        printf("[%d] ", *it);
    }
    puts("");

    int *pos = cstl_find(arr, 7);
    if (pos != NULL) {
        printf("find : [%3d]\n", *pos);
    }
	cstl_assign(arr, 5);
	///[0] [0] [0] [0] [0]
	for (int i = 0; i < cstl_size(arr); i++) {
		printf("[%d] ", arr[i]);
	}
	puts("");
	cstl_assign(arr, 5,7);
	///[7] [7] [7] [7] [7]
	for (int i = 0; i < cstl_size(arr); i++) {
		printf("[%d] ", arr[i]);
	}
	puts("");
    cstl_free(arr);
    Decorate("opencstl{vector} test end");
}
void cstl_vector_test2() {
	Decorate("opencstl{vector 2d} test begin");
	int** matrix = cstl_vector(int*);
	cstl_assign(matrix, 5);
	for (int i = 0; i < cstl_size(matrix); i++) {
		matrix[i] = cstl_vector(int);
		cstl_assign(matrix[i], 5);
	}

	for (int i = 0; i < cstl_size(matrix); i++) {
		for (int j = 0; j < cstl_size(matrix[i]); j++) {
			matrix[i][j] = i * j;
		}
	}
	for (int i = 0; i < cstl_size(matrix); i++) {
		for (int j = 0; j < cstl_size(matrix[i]); j++) {
			printf("[%3d]", matrix[i][j]);
		}
		puts("");
	}

	for (int i = 0; i < cstl_size(matrix); i++) {
		cstl_free(matrix[i]);
	}
	cstl_free(matrix);
	Decorate("opencstl{vector 2d} test end");
}
void cstl_list_test02() {
    Decorate("opencstl{list} test begin");
    int **list = cstl_list(int);
    for (int i = 0; i < 10; i++) {
        cstl_push_back(list, i);
    }
    ///[0] [1] [2] [3] [4] [5] [6] [7] [8] [9]
	printf("list size: %d\n", cstl_size(list));

    cstl_pop_back(list);
    ///[0] [1] [2] [3] [4] [5] [6] [7] [8]
    cstl_insert(list, cstl_begin(list), 777);
    ///[777] [0] [1] [2] [3] [4] [5] [6] [7] [8]
    cstl_insert(list, cstl_find(list, 4), 5, 999);
    ///[777] [0] [1] [2] [3] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
    cstl_erase(list, cstl_find(list, 2), cstl_find(list, 999));
    ///[777] [0] [1] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
    for (int *it = cstl_begin(list); it != cstl_end(list); it = cstl_next(it)) {
        printf("[%d] ", *it);
    }
    puts("");
	printf("list size: %d\n", cstl_size(list));
    printf("front : %d\n", cstl_front(list));
    printf("back : %d\n", cstl_back(list));

	cstl_resize(list, 20);
	printf("list size: %d\n", cstl_size(list));
	cstl_resize(list, 3);
	printf("list size: %d\n", cstl_size(list));
    cstl_free(list);
    Decorate("opencstl{list} test end");
}

int IntCmp(const void *a, const void *b) {
    return *(int *) a < *(int *) b ? -1 : *(int *) a > *(int *) b;
}
void cstl_set_test() {
    Decorate("opencstl{set} test begin");
    float** tree = cstl_set(float, IntCmp);
    for (float i = 0; i < 100; i++) {
        cstl_insert(tree, i);
    }
    /// [0] [1] ... [98] [99]
     for (float i = 50; i < 70; i++) {
    	int* it = cstl_find(tree, i);
    	cstl_erase(tree, it);
     }
    /// [0] [1] ... [48] [49] [70] [71] ... [98] [99]
    for (float *it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
        printf("[%f]", *it);
    }
    puts("");
	printf("size : %d\n", cstl_size(tree));
    cstl_free(tree);
    Decorate("opencstl{set} test end");
}
void cstl_map_test() {
    Decorate("opencstl{map} test begin");
    int** tree = cstl_map(int, float, IntCmp);
    for (int i = 0; i < 10; i++) {
        float d = (float) i;
        cstl_insert(tree, i, d * d);
    }
    ///[0]{0} [1]{1} [2]{4} [3]{9} [4]{16} [5]{25} [6]{36} [7]{49} [8]{64} [9]{81}
    for (int i = 0; i < 10; i += 2) {
        int *it = cstl_find(tree, i);
        cstl_erase(tree, it);
    }
    ///[1]{1} [3]{9} [5]{25} [7]{49} [9]{81}
    for (int *it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
        printf("[%3d]{%.3f} ", *it, cstl_value(it, float));
    }
    puts("");
	printf("size : %d\n", cstl_size(tree));
    cstl_free(tree);
    Decorate("opencstl{map} test end");
}

void cstl_deque_test() {
    Decorate("opencstl{deque} test begin");
    int *deque = cstl_deque(int);
    for (int i = 0; i < 10; i++) {
        cstl_push_back(deque, i);
    }
    for (int i = 0; i < 10; i++) {
        cstl_push_front(deque, i);
    }
    for (int i = 0; i < cstl_size(deque); i++) {
        printf("[%3d] ", deque[i]);
    }
    puts("");

    printf("front : %d\n", deque[0]);
    printf("front : %d\n", cstl_front(deque));
    printf("back : %d\n", cstl_back(deque));


	cstl_assign(deque, 5);
	///[0] [0] [0] [0] [0]
	printf("size: %d\n", cstl_size(deque));
	for (int i = 0; i < cstl_size(deque); i++) {
		printf("[%d] ", deque[i]);
	}
	puts("");
	cstl_assign(deque, 5, 7);
	cstl_resize(deque, 10);
	///[7] [7] [7] [7] [7]
	for (int i = 0; i < cstl_size(deque); i++) {
		printf("[%d] ", deque[i]);
	}
	puts("");


    cstl_free(deque);
    Decorate("opencstl{deque} test end");
}
void cstl_stack_test() {
    Decorate("opencstl{stack} test begin");
    int *stack = cstl_stack(int);
    for (int i = 0; i < 100; i++) {
        cstl_push(stack, i);
    }
    for (int i = 0; i < 100; i++) {
        printf("[%3d]", cstl_top(stack));
        cstl_pop(stack);
    }
    puts("");
    cstl_free(stack);
    Decorate("opencstl{stack} test end");
}
void cstl_queue_test() {
    Decorate("opencstl{queue} test begin");
    int *queue = cstl_queue(int);
    for (int i = 0; i < 100; i++) {
        cstl_push(queue, i);
    }
    for (int i = 0; i < 100; i++) {
        printf("[%3d]", cstl_front(queue));
        cstl_pop(queue);
    }
    puts("");
    cstl_free(queue);
    Decorate("opencstl{queue} test end");
}

void cstl_priority_queue_test() {
	Decorate("opencstl{priority_queue} test begin");
	int *queue = cstl_priority_queue(int,IntCmp);
	for (int i = 0; i < 10; i++) {
		cstl_push(queue, i);
	}
	while(!cstl_empty(queue)){
		printf("[%3d]", cstl_top(queue));
		cstl_pop(queue);
	}
	puts("");
	cstl_free(queue);
	Decorate("opencstl{priority_queue} test end");
}
typedef struct {
	int cost, to;
}Edge;
Edge make_edge(int cost, int to) { Edge r = { cost,to };return r;}
int compare_edge(const void* _a, const void* _b) {
	Edge* a = (Edge*)_a;
	Edge* b = (Edge*)_b;
	return a->cost > b->cost ? -1 : a->cost < b->cost;
}
int* dijkstra(Edge** vec, int begin) {
	int* d = cstl_vector(int);
	Edge* pqueue = cstl_priority_queue(Edge, compare_edge);
	cstl_assign(d, cstl_size(vec), 99999);
	d[begin] = 0;
	cstl_push(pqueue, make_edge(0, begin));
	while (!cstl_empty(pqueue)) {
		Edge e = cstl_top(pqueue);
		cstl_pop(pqueue);
		int end = cstl_size(vec[e.to]);
		for (int i = 0; i < end; i++) {
			int* a = &d[vec[e.to][i].to];
			int b = d[e.to];
			int c = vec[e.to][i].cost;
			if (*a > b + c) {
				*a = b + c;
				int value = vec[e.to][i].to;
				cstl_push(pqueue, make_edge(*a, value));
			}
		}
	}
	cstl_free(pqueue);
	return d;
}
void cstl_priority_queue_test2() {
	Edge** vec = cstl_vector(Edge*);
	cstl_assign(vec, 7);
	for (int i = 0; i < cstl_size(vec); i++) {
		vec[i] = cstl_vector(Edge);
	}
	cstl_push_back(vec[1], make_edge(10, 2));
	cstl_push_back(vec[1], make_edge(30, 3));
	cstl_push_back(vec[1], make_edge(15, 4));
	cstl_push_back(vec[2], make_edge(20,5));
	cstl_push_back(vec[3], make_edge(5,6));
	cstl_push_back(vec[4], make_edge(5,3));
	cstl_push_back(vec[4], make_edge(20,6));
	cstl_push_back(vec[5], make_edge(20, 6));
	cstl_push_back(vec[6], make_edge(20, 4));

	int* d = dijkstra(vec, 1);
	for (int i = 0; i < cstl_size(d); i++) {
		printf("%d\n", d[i]);
	}
	cstl_free(d);
	for (int i = 0; i < cstl_size(vec); i++) {
		cstl_free(vec[i]);
	}
	cstl_free(vec);
}
int main() {
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
    return 0;
}
