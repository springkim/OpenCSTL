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
//               Copyright (C) 2018, Woo Cheol, all rights reserved.
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
#if !defined(_OPENCSTL_PRIORITY_QUEUE_H)
#define _OPENCSTL_PRIORITY_QUEUE_H
#include"types.h"
#include"defines.h"
#include"error.h"

#define cstl_priority_queue(KEY,...)	__cstl_priority_queue(sizeof(KEY),#KEY,ARGN(__VA_ARGS__),__VA_ARGS__)
OPENCSTL_FUNC void* __cstl_priority_queue(size_t type_size, char* type, int argc, ...) {
	va_list vl;
	va_start(vl, argc);
	void* compare = va_arg(vl, void*);
	if (argc == 0) {
		compare = NULL;		//use default compare function(memcmp)
	}
	size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
	void* ptr = (char*)malloc(header_sz + sizeof(size_t)) + header_sz;
	void** container = &ptr;
	OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_PRIORITY_QUEUE;
	OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
	OPENCSTL_NIDX(container, NIDX_TSIZE) = type_size;
	OPENCSTL_NIDX(container, -4) = (size_t)type;					//type
	OPENCSTL_NIDX(container, -3) = (size_t)compare;
	OPENCSTL_NIDX(container, -2) = 1;	//capacity
	OPENCSTL_NIDX(container, -1) = 0;	//size
	return ptr;
}

OPENCSTL_FUNC void __cstl_priority_queue_swap(void* a, void* b, size_t type_size) {
	char* tmp = (char*)calloc(type_size, 1);
	memcpy(tmp, a, type_size);
	memcpy(a, b, type_size);
	memcpy(b, tmp, type_size);
}

OPENCSTL_FUNC void __cstl_priority_queue_heapifyD2U(void**container, size_t i) {
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
	cstl_compare compare = OPENCSTL_NIDX(container, -3);

	size_t idx = i;
	while (idx > 0 && compare((char*)*container + idx*type_size, (char*)*container + (idx - 1) / 2 * type_size) > 0) {
		__cstl_priority_queue_swap((char*)*container + idx*type_size, (char*)*container + (idx - 1) / 2 * type_size, type_size);
		idx = (idx - 1) / 2;
	}
}

OPENCSTL_FUNC void __cstl_priority_queue_heapifyU2D(void**container, size_t i) {
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
	size_t length = OPENCSTL_NIDX(container, -1);
	cstl_compare compare = OPENCSTL_NIDX(container, -3);
	size_t idx = i, c = idx;
	while (idx < length) {
		if (compare((char*)*container + idx*type_size, (char*)*container + (idx * 2 + 1)*type_size) < 0 && idx * 2 + 1 < length)
			c = idx * 2 + 1;
		if (compare((char*)*container + c*type_size, (char*)*container + (idx * 2 + 2)*type_size) < 0 && idx * 2 + 2 < length)
			c = idx * 2 + 2;
		if (c == idx)break;
		__cstl_priority_queue_swap((char*)*container + c*type_size, (char*)*container + idx*type_size, type_size);
		idx = c;
	}
}

OPENCSTL_FUNC void __cstl_priority_queue_push(void** container, void* value) {
	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
	size_t length = OPENCSTL_NIDX(container, -1);
	size_t capacity = OPENCSTL_NIDX(container, -2);

	char* type = (char*)OPENCSTL_NIDX(container, -4);
	float valuef = 0.0F;
	if (strcmp(type, "float") == 0) {
		valuef = (float)*(double*)value;
		value = &valuef;
	}
	if (length == capacity) {
		void* b = realloc((char*)*container - header_sz, header_sz + capacity * 2 * type_size);
		if (b == NULL) {
			cstl_error("Reallocation failed at priority_queue push");
		}
		*container = ((char*)b + header_sz);
		OPENCSTL_NIDX(container, -2) *= 2;
	}
	memcpy((char*)*container + type_size*length, value, type_size);
	OPENCSTL_NIDX(container, -1)++;
	//heapifyD2U
	while (length) {
		__cstl_priority_queue_heapifyD2U(container, length--);
	}
}
OPENCSTL_FUNC void __cstl_priority_queue_pop(void** container) {
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
	size_t length = OPENCSTL_NIDX(container, -1);
	if (OPENCSTL_NIDX(container, -1) <= 0) {
		cstl_error("No elements in cstl_priority_queue");
	}
	__cstl_priority_queue_swap(*container, (char*)*container + (length - 1)*type_size, type_size);
	OPENCSTL_NIDX(container, -1)--;
	//heapifyU2D
	size_t i = OPENCSTL_NIDX(container, -1) / 2;
	while (i--) {
		__cstl_priority_queue_heapifyU2D(container, i);
	}
}

#endif