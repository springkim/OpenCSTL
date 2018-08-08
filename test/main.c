#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"../opencstl/opencstl.h"
#define Decorate(STR)	for (int i = 0; i < 30; i++)putchar('='); printf(STR); for (int i = 0; i < 30; i++)putchar('=');putchar('\n');
void cstl_vector_test() {
	Decorate("opencstl{vector} test begin");
	int* arr = cstl_vector(int);

	//cstl_push_back(arr, 88);
	for (int i = 0; i < 10; i++) {
		cstl_push_back(arr, i);
	}

	///[0] [1] [2] [3] [4] [5] [6] [7] [8] [9]
	cstl_pop_back(arr);
	///[0] [1] [2] [3] [4] [5] [6] [7] [8]
	cstl_insert(arr, arr + 0, 777);
	///[777] [0] [1] [2] [3] [4] [5] [6] [7] [8]
	cstl_insert(arr, arr + 5, 5,999);
	///[777] [0] [1] [2] [3] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
	cstl_erase(arr, arr + 3, arr+5);

	///[777] [0] [1] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
	cstl_resize(arr, 15, -1);

	///[777] [0] [1] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8] [-1] [-1]
	for (int* it = cstl_begin(arr); it != cstl_end(arr); it++) {
		printf("[%d] ", *it);
	}
	puts("");

	int* pos = cstl_find(arr, 7);
	if(pos!=NULL) printf("find : [%3d]\n", *pos);
	cstl_free(arr);
	Decorate("opencstl{vector} test end");
}

void cstl_list_test02() {
	Decorate("opencstl{list} test begin");
	int** list = cstl_list(int);
	for (int i = 0; i < 10; i++) {
		cstl_push_back(list, i);
	}
	///[0] [1] [2] [3] [4] [5] [6] [7] [8] [9]
	cstl_pop_back(list);
	///[0] [1] [2] [3] [4] [5] [6] [7] [8]
	cstl_insert(list, cstl_begin(list), 777);
	///[777] [0] [1] [2] [3] [4] [5] [6] [7] [8]
	cstl_insert(list, cstl_find(list,4), 5, 999);
	///[777] [0] [1] [2] [3] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
	cstl_erase(list, cstl_find(list, 2), cstl_find(list, 999));
	///[777] [0] [1] [999] [999] [999] [999] [999] [4] [5] [6] [7] [8]
	for (int* it = cstl_begin(list); it != cstl_end(list); it = cstl_next(it)) {
		printf("[%d] ", *it);
	}
	puts("");
	printf("front : %d\n", list[0][0]);
	printf("front : %d\n", cstl_front(list));
	printf("back : %d\n", list[-2][0]);
	printf("back : %d\n", cstl_back(list));
	cstl_free(list);
	Decorate("opencstl{list} test end");
}

int IntCmp(const void* a, const void* b) {
	return *(int*)a < *(int*)b ? -1 : *(int*)a > *(int*)b;
}
void cstl_set_test() {
	Decorate("opencstl{set} test begin");
	cstl tree = cstl_set(float, IntCmp);
	for (float i = 0; i < 100; i++) {
		cstl_insert(tree, i);
	}
	/// [0] [1] ... [98] [99]
	//for (float i = 50; i < 70; i++) {
	//	int* it = cstl_find(tree, i);
	//	cstl_erase(tree, it);
	//}
	/// [0] [1] ... [48] [49] [70] [71] ... [98] [99]
	for (float* it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
		printf("[%f]", *it);
	}
	puts("");
	cstl_free(tree);
	Decorate("opencstl{set} test end");
}
void cstl_map_test() {
	Decorate("opencstl{map} test begin");
	cstl tree = cstl_map(int, float, IntCmp);
	for (int i = 0; i < 10; i++) {
		float d = (float)i;
		cstl_insert(tree, i, d*d);
	}
	///[0]{0} [1]{1} [2]{4} [3]{9} [4]{16} [5]{25} [6]{36} [7]{49} [8]{64} [9]{81}
	for (int i = 0; i < 10; i += 2) {
		int* it = cstl_find(tree, i);
		cstl_erase(tree, it);
	}
	///[1]{1} [3]{9} [5]{25} [7]{49} [9]{81}
	for (int* it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
		printf("[%3d]{%.3f} ", *it, cstl_value(it, float));
	}
	puts("");
	cstl_free(tree);
	Decorate("opencstl{map} test end");
}

void cstl_deque_test() {
	Decorate("opencstl{deque} test begin");
	int* deque = cstl_deque(int);
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
	cstl_free(deque);
	Decorate("opencstl{deque} test end");
}
void cstl_stack_test() {
	Decorate("opencstl{stack} test begin");
	int* stack = cstl_stack(int);
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
	int* queue = cstl_queue(int);
	for (int i = 0; i < 100; i++) {
		cstl_push(queue, i);
	}
	for (int i = 0; i < 100; i++) {
		printf("[%3d]", cstl_top(queue));
		cstl_pop(queue);
	}
	puts("");
	cstl_free(queue);
	Decorate("opencstl{stack} test end");
}

int main() {
	cstl_vector_test();
	cstl_list_test02();
	cstl_set_test();
	cstl_map_test();
	cstl_deque_test();
	cstl_stack_test();
	cstl_queue_test();
	return 0;
}
