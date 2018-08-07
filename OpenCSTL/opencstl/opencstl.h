#pragma once
#include"defines.h"
#include"error.h"
#include"types.h"
#include"vector.h"
#include"list.h"
#include"deque.h"
#include"tree.h"

intmax_t is_deque(void** container) {
	if (OPENCSTL_NIDX(container, -1) > INT_MAX) //음수 여부 판단
		return 1;
	return 0;
}

#define cstl_push_back(container,...)	_cstl_push_back(&(container),__VA_ARGS__)
void _cstl_push_back(void* container, ...) {
	va_list vl;
	va_start(vl, container);
	void* value = vl;
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	} else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			__cstl_vector_push_back((void**)container, value);
		}break;
		case OPENCSTL_LIST: {
			__cstl_list_push_back_front((void**)container, value,-1,0);
		}break;
		case OPENCSTL_DEQUE: {
			__cstl_deque_push_back((void**)container, value);
		}break;
		default:cstl_error("Invalid operator"); break;
	}
}
#define cstl_push_front(container,...)	_cstl_push_front(&(container),__VA_ARGS__)
void _cstl_push_front(void* container, ...) {
	va_list vl;
	va_start(vl, container);
	void* value = vl;
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_LIST: {
			__cstl_list_push_back_front((void**)container, value, 0, -1);
		}break;
		case OPENCSTL_DEQUE: {
			__cstl_deque_push_front((void**)container, value);
		}break;
		default:cstl_error("Invalid operator"); break;
	}
}
#define cstl_pop_back(container)	_cstl_pop_back(&(container))
void _cstl_pop_back(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			__cstl_vector_pop_back((void**)container);
		}break;
		case OPENCSTL_LIST: {
			__cstl_list_pop_back_front((void**)container, -1, 0);
		}break;
		case OPENCSTL_DEQUE: {
			__cstl_deque_pop_back((void**)container);
		}break;
	}
}
#define cstl_pop_front(container)	_cstl_pop_front(&(container))
void _cstl_pop_front(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_LIST: {
			__cstl_list_pop_back_front((void**)container, 0,-1);
		}break;
		case OPENCSTL_DEQUE: {
			__cstl_deque_pop_front((void**)container);
		}break;
	}
}
#define cstl_size(container)	_cstl_size(&(container))
size_t _cstl_size(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	size_t sz = 0;
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			sz=__cstl_vector_size((void**)container);
		}break;
		case OPENCSTL_LIST: {
			sz = __cstl_list_size((void**)container);
		}break;
			break;
		case OPENCSTL_DEQUE: {
			sz = __cstl_deque_size((void**)container);
		}break;
	}
	return sz;
}
#define cstl_next(iterator)	_cstl_next(iterator)
void* _cstl_next(void* it) {
	size_t node_type = OPENCSTL_NIDX(&it, -3);
	switch (node_type) {
		case OPENCSTL_LIST:{
			return __cstl_list_next_prev(it,-1);
		}break;
		case OPENCSTL_SET:
		case OPENCSTL_MAP: {
			return __cstl_tree_next_prev(it,-1,-2,__cstl_tree_toleft);
		}break;
		default:cstl_error("Invalid operation"); break;
	}
	return NULL;
}
#define cstl_prev(iterator)	_cstl_prev(iterator)
void* _cstl_prev(void* it) {
	size_t node_type = OPENCSTL_NIDX(&it, -3);
	switch (node_type) {
		case OPENCSTL_LIST: {
			return __cstl_list_next_prev(it,-2);
		}break;
		case OPENCSTL_SET:
		case OPENCSTL_MAP: {
			return __cstl_tree_next_prev(it, -2, -1, __cstl_tree_toright);
		}break;
		default:cstl_error("Invalid operation"); break;
	}
	return NULL;
}

#define cstl_insert(container,...)	_cstl_insert(&(container),ARGN(__VA_ARGS__),__VA_ARGS__)
void _cstl_insert(void* container,int argc, ...) {
	va_list vl;
	va_start(vl, argc);
	void* param1 = vl;
	void* param2 = (char*)param1 + sizeof(void*);
	void* param3 = (char*)param2 + sizeof(void*);
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			if (argc == 2)__cstl_vector_insert((void**)container, param1, 1, param2);
			else __cstl_vector_insert((void**)container, param1, *(size_t*)param2, param3);
		}break;
		case OPENCSTL_LIST: {
			if(argc==2)__cstl_list_insert((void**)container, param1,1, param2);
			else __cstl_list_insert((void**)container, param1, *(size_t*)param2, param3);
		}break;
		case OPENCSTL_DEQUE: {
			if(argc==2)__cstl_deque_insert((void**)container, param1, 1, param2);
			else __cstl_deque_insert((void**)container, param1, *(size_t*)param2, param3);
		}break;
		case OPENCSTL_MAP: {
			if(argc==2) __cstl_tree_insert((void**)container, param1, param2);
			else cstl_error("Invalid operation");
		}break;
		case OPENCSTL_SET: {
			if(argc==1) __cstl_tree_insert((void**)container, param1,NULL);
			else cstl_error("Invalid operation");
		}break;
	}
}
#define cstl_erase(container,...)	_cstl_erase(&(container),ARGN(__VA_ARGS__),__VA_ARGS__)
void _cstl_erase(void* container, int argc, ...) {
	va_list vl;
	va_start(vl, argc);
	void* param1 = vl;
	void* param2 = (char*)param1 + sizeof(void*);
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			if (argc == 1) {
				param2 = (*(char**)param1)+ OPENCSTL_NIDX((void**)container, NIDX_TSIZE);
				__cstl_vector_erase((void**)container, param1, &param2);
			} else __cstl_vector_erase((void**)container, param1, param2);
		}break;
		case OPENCSTL_LIST: {
			if (argc == 1) {
				param2 = cstl_next(*(void**)param1);
				__cstl_list_erase((void**)container, param1, &param2);
			}
			else __cstl_list_erase((void**)container, param1, param2);
		}break;
		case OPENCSTL_DEQUE: {
			if (argc == 1) {
				param2 = (*(char**)param1) + OPENCSTL_NIDX((void**)container, NIDX_TSIZE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
				__cstl_deque_erase((void**)container, param1, &param2);
			} else __cstl_deque_erase((void**)container, param1, param2);
		}break;
		case OPENCSTL_MAP:
		case OPENCSTL_SET: {
			__cstl_tree_erase((void**)container,*(void**) param1);
		}break;
	}
}
#define cstl_resize(container,...)	_cstl_resize(&(container),ARGN(__VA_ARGS__),__VA_ARGS__)
void _cstl_resize(void* container, int argc, ...) {
	va_list vl;
	va_start(vl, argc);
	void* param1 = vl;
	void* param2 = (char*)param1 + sizeof(void*);
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			if (argc == 1)param2 = NULL;
			__cstl_vector_resize((void**)container, *(size_t*)param1, param2);
		}break;
		case OPENCSTL_LIST: {
			
		}break;
		case OPENCSTL_DEQUE: {
			if (argc == 1)param2 = NULL;
			__cstl_deque_resize((void**)container, *(size_t*)param1, &param2);
		}break;
		case OPENCSTL_SET: {

		}break;
	}
}
#define cstl_begin(container)	_cstl_begin(&(container))
void* _cstl_begin(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			return __cstl_vector_begin((void**)container);
		}break;
		case OPENCSTL_LIST: {
			return __cstl_list_begin((void**)container);
		}break;
		case OPENCSTL_DEQUE: {
			return __cstl_deque_begin((void**)container);
		}break;
		case OPENCSTL_SET:
		case OPENCSTL_MAP: {
			return __cstl_tree_begin((void**)container);
		}break;
		default:cstl_error("Invalid operation"); break;
	}
	return NULL;
}
#define cstl_rbegin(container)	_cstl_rbegin(&(container))
void* _cstl_rbegin(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			return __cstl_vector_rbegin((void**)container);
		}break;
		case OPENCSTL_LIST: {
			return __cstl_list_rbegin((void**)container);
		}break;
		case OPENCSTL_DEQUE: {
			return __cstl_deque_rbegin((void**)container);
		}break;
		case OPENCSTL_SET:
		case OPENCSTL_MAP: {
			return __cstl_tree_rbegin((void**)container);
		}break;
		default:cstl_error("Invalid operation"); break;
	}
	return NULL;
}
#define cstl_end(container)	_cstl_end(&(container))
void* _cstl_end(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			return __cstl_vector_end((void**)container);
		}break;
		case OPENCSTL_LIST: {
			return __cstl_list_end_rend((void**)container);
		}break;
		case OPENCSTL_DEQUE: {
			return __cstl_deque_end((void**)container);
		}break;
		case OPENCSTL_SET:
		case OPENCSTL_MAP: {
			return __cstl_tree_end_rend((void**)container);
		}break;
		default:cstl_error("Invalid operation"); break;
	}
	return NULL;
}
#define cstl_rend(container)	_cstl_rend(&(container))
void* _cstl_rend(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			return __cstl_vector_rend((void**)container);
		}break;
		case OPENCSTL_LIST: {
			return __cstl_list_end_rend((void**)container);
		}break;
		case OPENCSTL_DEQUE: {
			return __cstl_deque_rend((void**)container);
		}break;
		case OPENCSTL_SET:
		case OPENCSTL_MAP: {
			return __cstl_tree_end_rend((void**)container);
		}break;
		default:cstl_error("Invalid operation"); break;
	}
	return NULL;
}
#define cstl_clear(container)	_cstl_clear(&(container))
void _cstl_clear(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {

		}break;
		case OPENCSTL_LIST: {
			__cstl_list_clear((void**)container);
		}break;
	}
}
#define cstl_find(container,...)	_cstl_find(&(container),ARGN(__VA_ARGS__),__VA_ARGS__)
void* _cstl_find(void* container,int argc, ...) {
	va_list vl;
	va_start(vl, argc);
	void* param1 = vl;
	void* param2 = (char*)param1 + sizeof(void*);
	size_t container_type;
	if (is_deque((void**)container)) {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE + (intmax_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			if (argc == 1)  return __cstl_vector_find((void**)container, container, param1);
			else return __cstl_vector_find((void**)container, param1,param2);
		}break;
		case OPENCSTL_LIST: {
			if (argc == 1) return __cstl_list_find((void**)container, (void**)&OPENCSTL_NIDX((void**)container, 0),param1);
			else return __cstl_list_find((void**)container, param1,param2);
		}break;
		case OPENCSTL_DEQUE: {
			if (argc == 1)  return __cstl_deque_find((void**)container, container, param1);
			else return __cstl_deque_find((void**)container, param1, param2);
		}break;
		case OPENCSTL_MAP:
		case OPENCSTL_SET: {
			return __cstl_tree_find((void**)container, param1);
		}break;
		default:cstl_error("Invalid operator"); break;
	}
	return NULL;
}