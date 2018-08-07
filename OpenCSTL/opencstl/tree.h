//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                               License Agreement
//                Open Source C Container Library like STL in C++
//
//               Copyright (C) 2018, Kim Bomm, all rights reserved.
//
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
#pragma once
#if !defined(_OPENCSTL_TREE_H)
#define _OPENCSTL_TREE_H
#include"types.h"
#include"error.h"
#include"defines.h"

#define P	-4
#define R -1
#define L -2
#define RED ((void*)1)
#define BLACK ((void*)0)
#ifdef _MSC_VER
#define _(N,V)	((void*)OPENCSTL_NIDX(&N,V))
#else
#define _(N,V)	OPENCSTL_NIDX(&N,V)
#endif
#define COLOR(N)	_(N,-5)


char nil_buffer[sizeof(void*) * 5] = { 0 };
void* nil = nil_buffer + sizeof(void*) * 5;
void* __cstl_tree_node(size_t type_size,size_t node_type) {
	//[color][parent][node type][left][right] ¢Ù [data]
	size_t node_sz = type_size + sizeof(void*) * 5;
	void* ptr = (char*)calloc(node_sz,1) + sizeof(void*) * 5;
	OPENCSTL_NIDX(&ptr, -3) = node_type;
	COLOR(ptr) = BLACK;
	return ptr;
}
#define cstl_set(KEY,...)	__cstl_set(sizeof(KEY),ARGN(__VA_ARGS__),__VA_ARGS__)
void* __cstl_set(size_t key_size,int argc, ...) {
	va_list vl;
	va_start(vl, argc);
	void* compare = *(void**)vl;
	if (argc == 0) {
		compare = NULL;		//use default compare function(memcmp)
	}
	size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
	void* ptr = (char*)malloc(header_sz + sizeof(size_t)) + header_sz;
	void** container = &ptr;
	OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_SET;
	OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
	OPENCSTL_NIDX(container, NIDX_TSIZE) = key_size;
	OPENCSTL_NIDX(container, -4) = 0;					//value size, but set does not have value.
	OPENCSTL_NIDX(container, -3) = 0;					//not-reserved
	OPENCSTL_NIDX(container, -2) = (size_t)compare;	//compare function
	OPENCSTL_NIDX(container, -1) = 0;
	OPENCSTL_NIDX(container, 0) = (size_t)nil;			//root
	return ptr;
}
#define cstl_map(KEY,VALUE,...)	__cstl_map(sizeof(KEY),sizeof(VALUE),ARGN(__VA_ARGS__),__VA_ARGS__)
void* __cstl_map(size_t key_size, size_t value_size,int argc, ...) {
	va_list vl;
	va_start(vl, argc);
	void* compare = *(void**)vl;
	if (argc == 0) {
		compare = NULL;		//use default compare function(memcmp)
	}
	size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
	void* ptr = (char*)malloc(header_sz + sizeof(size_t)) + header_sz;
	void** container = &ptr;
	OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_MAP;
	OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
	OPENCSTL_NIDX(container, NIDX_TSIZE) = key_size;
	OPENCSTL_NIDX(container, -4) = value_size;					//value size, but set does not have value.
	OPENCSTL_NIDX(container, -3) = 0;					//not-reserved
	OPENCSTL_NIDX(container, -2) = (size_t)compare;	//compare function
	OPENCSTL_NIDX(container, -1) = 0;
	OPENCSTL_NIDX(container, 0) = (size_t)nil;			//root
	return ptr;
}

void __cstl_tree_left_rotate(void** container, void* x) {
	void*** root = (void***)*container;
	void* y = _(x,R);
	_(x, R) = _(y, L);
	if (_(y,L) != nil) {
		_(_(y, L), P) = x;
	}
	_(y, P) = _(x, P);
	if (_(x,P) == nil) {
		*root = y;
	} else if (x == _(_(x,P),L)) {
		_(_(x, P), L) = y;
	} else {
		_(_(x, P), R) = y;
	}
	_(y, L) = x;
	_(x, P) = y;
}
void __cstl_tree_right_rotate(void** container, void* x) {
	void*** root = (void***)*container;
	void* y = _(x, L);
	_(x, L) = _(y, R);
	if (_(y, R) != nil) {
		_(_(y, R), P) = x;
	}
	_(y, P) = _(x, P);
	if (_(x, P) == nil) {
		*root = y;
	} else if (x == _(_(x, P), R)) {
		_(_(x, P), R) = y;
	} else {
		_(_(x, P), L) = y;
	}
	_(y, R) = x;
	_(x, P) = y;
}
void __cstl_tree_insert_fixup(void** container, void* z) {
	while (COLOR(_(z, P)) == RED) {
		if (_(z, P) == _(_(_(z, P), P), L)) {
			void* y = _(_(_(z, P), P), R);
			if (COLOR(y) == RED) {
				COLOR(_(z, P)) = BLACK;
				COLOR(y) = BLACK;
				COLOR(_(_(z, P), P)) = RED;
				z = _(_(z, P), P);
			} else {
				if (z == _(_(z, P), R)) {
					z = _(z, P);
					__cstl_tree_left_rotate(container, z);
				}
				COLOR(_(z, P)) = BLACK;
				COLOR(_(_(z, P),P)) = RED;
				__cstl_tree_right_rotate(container, _(_(z, P), P));
			}
		} else {
			void* y = _(_(_(z, P), P), L);
			if (COLOR(y) == RED) {
				COLOR(_(z, P)) = BLACK;
				COLOR(y) = BLACK;
				COLOR(_(_(z, P), P)) = RED;
				z = _(_(z, P), P);
			} else {
				if (z == _(_(z, P), L)) {
					z = _(z, P);
					__cstl_tree_right_rotate(container, z);
				}
				COLOR(_(z, P)) = BLACK;
				COLOR(_(_(z, P), P)) = RED;
				__cstl_tree_left_rotate(container, _(_(z, P), P));
			}
		}
	}
	void*** root = (void***)*container;
	COLOR(*root) = BLACK;
}

void __cstl_tree_insert(void** container, void* key,void* value) {
	size_t container_type=OPENCSTL_NIDX(container, NIDX_CTYPE);
	size_t header_sz=OPENCSTL_NIDX(container, NIDX_HSIZE);
	size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
	size_t value_size = OPENCSTL_NIDX(container, -4);
	size_t type_size = key_size + value_size;
	cstl_compare compare = (cstl_compare)OPENCSTL_NIDX(container, -2);

	void*** root = (void***)*container;
	void* n = __cstl_tree_node(type_size,container_type);
	memcpy(n, key, key_size);
	if (value) {
		memcpy((char*)n + key_size, value, value_size);
	}
	void* p = nil;
	while (*root!=nil) {
		p = *root;
		int r = compare? compare(*root, n) : memcmp(*root, n, type_size);
		if (r == 0) {
			return;
		} else if (r > 0) {	//to left
			root = (void***)&OPENCSTL_NIDX(root, L);
		} else {				//to right
			root = (void***)&OPENCSTL_NIDX(root, R);
		}
	}
	OPENCSTL_NIDX(&n, P) = (size_t)p;
	OPENCSTL_NIDX(&n, L) = (size_t)nil;
	OPENCSTL_NIDX(&n, R) = (size_t)nil;
	COLOR(n) = RED;
	
	*root = (void**)n;
	__cstl_tree_insert_fixup(container, n);
}

void __cstl_tree_transplant(void** container, void* u, void* v) {
	void*** root = (void***)*container;
	if (_(u, P) == nil) {
		*root = v;
	} else if (u == _(_(u, P), L)) {
		_(_(u, P), L) = v;
	} else {
		_(_(u, P), R) = v;
	}
	_(v, P) = _(u, P);
}
void* __cstl_tree_toleft(void* n) {
	while (_(n, L)!=nil) {	
		n = _(n, L);
	}
	return n;
}
void* __cstl_tree_toright(void* n) {
	while (_(n, R) != nil) {
		n = _(n, R);
	}
	return n;
}
void __cstl_tree_erase_fixup(void** container, void* x) {
	void*** root = (void***)*container;
	while (x != *root && COLOR(x) == BLACK) {
		intmax_t expression = (x == _(_(x, P), L));
		intmax_t left = expression ? L : R;
		intmax_t right = expression ? R : L;
		void* w = _(_(x, P), right);
		if (COLOR(w) == RED) {
			COLOR(w) = BLACK;
			COLOR(_(x, P)) = RED;
			__cstl_tree_left_rotate(container, _(x, P));
			w= _(_(x, P), right);
		}
		if (COLOR(_(w, left)) == BLACK && COLOR(_(w, right)) == BLACK) {
			COLOR(w) = RED;
			x = _(x, P);
		} else {
			if (COLOR(_(w, right)) == BLACK) {
				COLOR(_(w, left)) = BLACK;
				COLOR(w) = RED;
				__cstl_tree_right_rotate(container, w);
				w = _(_(x, P), right);
			}
			COLOR(w) = COLOR(_(x, P));
			COLOR(_(x, P)) = BLACK;
			COLOR(_(w, right)) = BLACK;
			__cstl_tree_left_rotate(container, _(x, P));
			x = *root;
		}
	}
	COLOR(x) = BLACK;
}
void __cstl_tree_erase(void** container, void** iter) {
	if (iter == NULL)return;
	size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
	size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
	size_t value_size = OPENCSTL_NIDX(container, -4);
	size_t type_size = key_size + value_size;
	cstl_compare compare = (cstl_compare)OPENCSTL_NIDX(container, -2);
	void*** root = (void***)*container;
	void* z = iter;

	void* y = z;
	void* x = NULL;
	size_t y_original_color = (size_t)COLOR(y);
	if (_(z, L) == nil) {
		x = _(z, R);
		__cstl_tree_transplant(container, z, _(z, R));
	} else if (_(z, R) == nil) {
		x = _(z, L);
		__cstl_tree_transplant(container, z, _(z, L));
	} else {
		y = __cstl_tree_toleft(_(z, R));
		y_original_color = (size_t)COLOR(y);
		x = _(y, R);
		if (_(y, P) == z) {
			_(x, P) = y;
		} else {
			__cstl_tree_transplant(container, y, _(y, R));
			_(y, R) = _(z, R);
			_(_(y, R), P) = y;
		}
		__cstl_tree_transplant(container, z, y);
		_(y, L) = _(z, L);
		_(_(y, L), P) = y;
		COLOR(y) = COLOR(z);
	}
	if (y_original_color == (size_t)BLACK) {
		__cstl_tree_erase_fixup(container, x);
	}
	printf("deleted[%d]\n", *(int*)iter);
	free(&OPENCSTL_NIDX(&iter, -5));
}

void* __cstl_tree_find(void** container, void* key) {
	size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
	size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
	size_t value_size = OPENCSTL_NIDX(container, -4);
	size_t type_size = key_size + value_size;
	cstl_compare compare = (cstl_compare)OPENCSTL_NIDX(container, -2);
	void*** root = (void***)*container;
	while (*root != nil) {
		int r = compare ? compare(*root, key) : memcmp(*root, key, type_size);
		if (r == 0) {
			return *root;
		} else if (r > 0) {	//to left
			root = (void***)&OPENCSTL_NIDX(root, L);
		} else {				//to right
			root = (void***)&OPENCSTL_NIDX(root, R);
		}
	}
	return NULL;
}

void* __cstl_tree_begin(void** container) {
	void*** root = (void***)*container;
	return **root ? __cstl_tree_toleft(*root) : nil;
}
void* __cstl_tree_rbegin(void** container) {
	void*** root = (void***)*container;
	return **root ? __cstl_tree_toright(*root) : nil;
}
void* __cstl_tree_end_rend(void** container) {
	return nil;
}
void* __cstl_tree_next_prev(void* it,int r,int l,void*(todeep)(void*)) {
	//next = r(-1) , l(-2)
	//prev = r(-2), l(-1)
	if (_(it, r) != nil) {
		it = todeep(_(it,r));
	} else if (_(it, P)!=nil) {
		if (_(_(it, P), l) == it) {
			it = _(it, P);
		} else {
			while (_(it, P)!=nil && _(_(it, P), r) == it) {
				it = _(it, P);
			}
			it = _(it, P);
		}
	}
	return it;
}

#undef P
#undef L
#undef R
#undef _
#undef RED
#undef BLACK


#endif