#pragma once
#include"defines.h"
#include"error.h"
#include"types.h"
#include"vector.h"
#include"list.h"
#include"deque.h"
#include"tree.h"
#include"stack.h"
#include"queue.h"




ptrdiff_t is_deque(void** container) {
	if (OPENCSTL_NIDX(container, -1) > INT_MAX)
		return 1;
	return 0;
}



void _cstl_push(void* container, ...) {
	va_list vl;
	void* va_ptr=NULL;
	__cstl_va_start(vl,container,va_ptr);
	void* value = __cstl_va_arg(va_ptr);
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
	case OPENCSTL_STACK:
	case OPENCSTL_QUEUE: {
		__cstl_deque_push_back((void**)container, value);
	}break;
	default:cstl_error("Invalid operator"); break;
	}
}

void _cstl_push_back(void* container, ...) {
	va_list vl;
	void* va_ptr=NULL;
	__cstl_va_start(vl,container,va_ptr);
	void* param1 = __cstl_va_arg(va_ptr);

	size_t container_type;

	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}

	switch (container_type) {
		case OPENCSTL_VECTOR: {
			__cstl_vector_push_back((void**)container, param1);
		}break;
		case OPENCSTL_LIST: {
			__cstl_list_push_back_front((void**)container, param1,-1,0);
		}break;
		case OPENCSTL_DEQUE: {
			__cstl_deque_push_back((void**)container, param1);
		}break;
		default:cstl_error("Invalid operator"); break;
	}
}

void _cstl_push_front(void* container, ...) {
	va_list vl;
	void* va_ptr=NULL;
	__cstl_va_start(vl,container,va_ptr);
	void* param1 = __cstl_va_arg(va_ptr);
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_LIST: {
			__cstl_list_push_back_front((void**)container, param1, 0, -1);
		}break;
		case OPENCSTL_DEQUE: {
			__cstl_deque_push_front((void**)container, param1);
		}break;
		default:cstl_error("Invalid operator"); break;
	}
}

void _cstl_pop(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	}
	else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
	case OPENCSTL_STACK: {
		__cstl_deque_pop_back((void**)container);
	}break;
	case OPENCSTL_QUEUE: {
		__cstl_deque_pop_front((void**)container);
	}break;
	default:cstl_error("Invalid operator"); break;
	}
}

void _cstl_pop_back(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
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

void _cstl_pop_front(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
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

size_t _cstl_size(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
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
		case OPENCSTL_SET:
		case OPENCSTL_MAP: {
			sz = __cstl_tree_size((void**)container);
		}break;
		case OPENCSTL_DEQUE: {
			sz = __cstl_deque_size((void**)container);
		}break;
		default:cstl_error("Invalid operation"); break;
	}
	return sz;
}

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


void _cstl_insert(void* container,int argc, ...) {
	va_list vl;
	void* va_ptr=NULL;
	__cstl_va_start(vl,argc,va_ptr);
	void* param1 = __cstl_va_arg(va_ptr);
	void* param2 = __cstl_va_arg((char*)va_ptr + sizeof(void*)*1);
	void* param3 = __cstl_va_arg((char*)va_ptr + sizeof(void*)*2);
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			if (argc == 2)__cstl_vector_insert((void**)container, param1, 1, param2);
			else __cstl_vector_insert((void**)container, param1, *(int*)param2, param3);
		}break;
		case OPENCSTL_LIST: {
			if(argc==2)__cstl_list_insert((void**)container, param1,1, param2);
			else __cstl_list_insert((void**)container, param1, *(int*)param2, param3);
		}break;
		case OPENCSTL_DEQUE: {
			if(argc==2)__cstl_deque_insert((void**)container, param1, 1, param2);
			else __cstl_deque_insert((void**)container, param1, *(int*)param2, param3);
		}break;
		case OPENCSTL_MAP: {
			if(argc==2) __cstl_tree_insert((void**)container, param1, param2);
			else cstl_error("Invalid operation");
		}break;
		case OPENCSTL_SET: {
			if(argc==1) __cstl_tree_insert((void**)container, param1,NULL);
			else cstl_error("Invalid operation");
		}break;
		default:cstl_error("Invalid operation"); break;
	}
}

void _cstl_erase(void* container, int argc, ...) {
	va_list vl;
	void* va_ptr=NULL;
	__cstl_va_start(vl,argc,va_ptr);
	void* param1 = __cstl_va_arg(va_ptr);
	void* param2 = __cstl_va_arg((char*)va_ptr + sizeof(void*)*1);

	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
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
				param2 = (*(char**)param1) + OPENCSTL_NIDX((void**)container, NIDX_TSIZE + (ptrdiff_t)OPENCSTL_NIDX(((void**)container), -1) + 1);
				__cstl_deque_erase((void**)container, param1, &param2);
			} else __cstl_deque_erase((void**)container, param1, param2);
		}break;
		case OPENCSTL_MAP:
		case OPENCSTL_SET: {
			__cstl_tree_erase((void**)container,*(void**) param1);
		}break;
		default:cstl_error("Invalid operation"); break;
	}
}

void _cstl_resize(void* container, int argc, ...) {
	va_list vl;
	void* va_ptr=NULL;
	__cstl_va_start(vl,argc,va_ptr);
	void* param1 = __cstl_va_arg(va_ptr);
	void* param2 = __cstl_va_arg((char*)va_ptr + sizeof(void*)*1);

	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			if (argc == 1)param2 = NULL;
			__cstl_vector_resize((void**)container, *(int*)param1, param2);
		}break;
		case OPENCSTL_LIST: {

		}break;
		case OPENCSTL_DEQUE: {
			if (argc == 1)param2 = NULL;
			__cstl_deque_resize((void**)container, *(int*)param1, &param2);
		}break;
		default:cstl_error("Invalid operation"); break;
	}
}

void* _cstl_begin(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
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

void* _cstl_rbegin(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
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

void* _cstl_end(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
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

void* _cstl_rend(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
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

void _cstl_clear(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			__cstl_vector_clear((void**)container);
		}break;
		case OPENCSTL_LIST: {
			__cstl_list_clear((void**)container);
		}break;
		case OPENCSTL_STACK:
		case OPENCSTL_QUEUE:
		case OPENCSTL_DEQUE: {
			__cstl_deque_clear((void**)container);
		}break;
		case OPENCSTL_SET:
		case OPENCSTL_MAP: {
			__cstl_tree_clear((void**)container);
		}break;
		default:cstl_error("Invalid operation"); break;
	}
}
bool _cstl_empty(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	size_t sz = 0;
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			sz = __cstl_vector_size((void**)container);
		}break;
		case OPENCSTL_LIST: {
			sz = __cstl_list_size((void**)container);
		}break;
		case OPENCSTL_STACK:
		case OPENCSTL_QUEUE:
		case OPENCSTL_DEQUE: {
			sz = __cstl_deque_size((void**)container);
		}break;
	}
	return sz ? true : false;
}
void _cstl_free(void* container) {
	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
		container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
	}
	switch (container_type) {
		case OPENCSTL_VECTOR: {
			__cstl_vector_free((void**)container);
		}break;
		case OPENCSTL_LIST: {
			__cstl_list_free((void**)container);
		}break;
		case OPENCSTL_STACK:
		case OPENCSTL_QUEUE:
		case OPENCSTL_DEQUE: {
			__cstl_deque_free((void**)container);
		}break;
		case OPENCSTL_SET:
		case OPENCSTL_MAP: {
			__cstl_tree_free((void**)container);
		}break;
		default:cstl_error("Invalid operation"); break;
	}
}

void* _cstl_find(void* container,int argc, ...) {
	va_list vl;
	void* va_ptr=NULL;
	__cstl_va_start(vl,argc,va_ptr);
	void* param1 = __cstl_va_arg(va_ptr);
	void* param2 = __cstl_va_arg((char*)va_ptr + sizeof(void*)*1);


	size_t container_type;
	if (is_deque((void**)container)) {
		ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
		container_type = *(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + distance);
	} else {
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
