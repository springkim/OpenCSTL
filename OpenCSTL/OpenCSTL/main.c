#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"opencstl/opencstl.h"
void cstl_vector_test() {
	int* arr = cstl_vector(int);
	for (int i = 0; i < 10; i++) {
		cstl_push_back(arr, i);
	}
	cstl_pop_back(arr);
	for (int i = 0; i < cstl_size(arr); i++) {
		printf("[%3d] ", arr[i]);
	}
	puts("");

	cstl_insert(arr, arr + 0, 777);
	cstl_insert(arr, arr + 5, 5,999);
	for (int* it = cstl_begin(arr); it != cstl_end(arr); it++) {
		printf("[%3d] ", *it);
	}
	puts("");

	//cstl_erase(arr, arr + 0,arr+cstl_size(arr));

	cstl_resize(arr, 18,-1);
	for (int i = 0; i < cstl_size(arr); i++) {
		printf("[%3d] ", arr[i]);
	}
	puts("");

	int* pos = cstl_find(arr, 7);
	printf("%d\n", *pos);
}

void cstl_list_test02() {
	int** list = cstl_list(int);
	for (int i = 0; i < 10; i++) {
		cstl_push_back(list, i*100);
	}
	for (int* it = cstl_begin(list); it != cstl_end(list); it = cstl_next(it)) {
		printf("%d ", *it);
	}
	puts("");
	
	//cstl_erase(&list, cstl_begin(&list), cstl_end(&list));

	int* pos = cstl_find(list, 500);

	cstl_insert(list, cstl_begin(list), 2,8);
	cstl_insert(list, pos, 7);
	cstl_insert(list, pos, 6);
	cstl_insert(list, cstl_end(list), 999);
	
	//cstl_clear(&list);

	for (int* it = cstl_rbegin(list); it != cstl_rend(list); it = cstl_prev(it)) {
		printf("%d ", *it);
	}
	puts("");
}

int IntCmp(const void* a, const void* b) {
	return *(int*)a < *(int*)b ? -1 : *(int*)a > *(int*)b;
}
void cstl_set_test() {
	int** tree = cstl_set(int, IntCmp);
	
	for (int i = 0; i < 1000; i++) {
		cstl_insert(tree,rand()%1000);
	}
	
	for (int* it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
		printf("[%d]", *it);
	}
	puts("");;
	for (int i = 0; i < 1000; i++) {
		int* it = cstl_find(tree, i);
		cstl_erase(tree, it);
	}

	for (int* it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
		printf("[%d]", *it);
	}
	puts("");
}
void cstl_map_test() {
	int** tree = cstl_map(int, double,IntCmp);

	for (int i = 0; i < 10; i++) {
		double d = (double)i;
		cstl_insert(tree, i,d*d);
	}
	for (int* it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
		printf("[%d]", *it);
	}
	puts("");

	for (int* it = cstl_rbegin(tree); it != cstl_rend(tree); it = cstl_prev(it)) {
		printf("[%d]", *it);
	}
	puts("");

	for (int i = 0; i < 10; i++) {
		int* it = cstl_find(tree, i);
		printf("%f\n", cstl_value(it,double));
		cstl_erase(tree, it);
	}
	for (int* it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
		printf("[%d]", *it);
	}
	puts("");
}
int main() {
	cstl_vector_test();
	
	cstl_list_test02();
	cstl_set_test();
	cstl_map_test();
}