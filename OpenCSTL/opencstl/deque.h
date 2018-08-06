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
#if !defined(_OPENCSTL_DEQUE_H)
#define _OPENCSTL_DEQUE_H
#include"types.h"
#include"defines.h"
#include"error.h"

#define cstl_deque(TYPE)	__cstl_deque(sizeof(TYPE))
void* __cstl_deque(size_t type_size) {
	size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
	void* ptr = (char*)malloc(header_sz + type_size * 2) + header_sz; // 2 = capacity
	void** container = &ptr;
	OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_DEQUE;
	OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
	OPENCSTL_NIDX(container, NIDX_TSIZE) = type_size;
	OPENCSTL_NIDX(container, -3) = 2;	//capacity
	OPENCSTL_NIDX(container, -2) = 0;	//length
	OPENCSTL_NIDX(container, 0) = -2;	//distance 가 없을 경우 -1
	return (void*)((char*)ptr + type_size);
}
void __cstl_deque_push_back(void** container, void* value) {

	intmax_t distance = OPENCSTL_NIDX(container, -1) + 1;

	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE + distance);
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE + distance);
	size_t length = OPENCSTL_NIDX(container, -2 + distance);
	size_t capacity = OPENCSTL_NIDX(container, -3 + distance);
	if (length == capacity + distance) {
		size_t distance_sz = (-distance)*type_size;
		void* b = calloc(1, header_sz + capacity * 2 * type_size);
		memcpy(b, (char*)*container - (header_sz + distance_sz), header_sz);
		distance = capacity * 2 / 4;
		memcpy((char*)b + header_sz + distance*type_size, *container, length*type_size);
		free((char*)*container - (header_sz + distance_sz));
		*container = ((char*)b + (header_sz + distance*type_size));
		OPENCSTL_NIDX(container, -3 - distance) *= 2;
		OPENCSTL_NIDX(container, -1) = -distance - 1;
	}
	memcpy((char*)*container + type_size*length, value, type_size);
	OPENCSTL_NIDX(container, -2 + OPENCSTL_NIDX(container, -1) + 1)++;
}
void __cstl_deque_push_front(void** container, void* value) {
	intmax_t distance = OPENCSTL_NIDX(container, -1) + 1;

	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE + distance);
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE + distance);
	size_t length = OPENCSTL_NIDX(container, -2 + distance);
	size_t capacity = OPENCSTL_NIDX(container, -3 + distance);
	if (distance == 0) {
		void* b = calloc(1, header_sz + capacity * 2 * type_size);
		memcpy(b, (char*)*container - header_sz, header_sz);
		distance = capacity * 2 / 4;
		memcpy((char*)b + header_sz + distance*type_size, *container, length*type_size);
		free((char*)*container - header_sz);
		*container = ((char*)b + (header_sz + distance*type_size));
		OPENCSTL_NIDX(container, -3 - distance) *= 2;
		distance = -distance;
	}
	memcpy((char*)*container - type_size * 2, (char*)*container - type_size, type_size);
	memcpy((char*)*container - type_size, value, type_size);
	*container = (char*)*container - type_size;
	OPENCSTL_NIDX(container, -1) = distance;
	OPENCSTL_NIDX(container, -2 + OPENCSTL_NIDX(container, -1) + 1)++;
}
void __cstl_deque_pop_back(void** container) {
	if (OPENCSTL_NIDX(container, -2 + OPENCSTL_NIDX(container, -1) + 1) <= 0) {
		cstl_error("No elements in cstl_deque");
	}
	OPENCSTL_NIDX(container, -2 + OPENCSTL_NIDX(container, -1) + 1)--;
}
void __cstl_deque_pop_front(void** container) {
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE + OPENCSTL_NIDX(container, -1) + 1);
	if (OPENCSTL_NIDX(container, -2 + OPENCSTL_NIDX(container, -1) + 1) <= 0) {
		cstl_error("No elements in cstl_deque");
	}
	memcpy(*container, (char*)*container - type_size, type_size);
	*container = (char*)*container + type_size;
	OPENCSTL_NIDX(container, -1)--;
	OPENCSTL_NIDX(container, -2 + OPENCSTL_NIDX(container, -1) + 1)--;
}

void __cstl_deque_insert(void** container, void* it, size_t n, void* value) {
	intmax_t distance = OPENCSTL_NIDX(container, -1) + 1;

	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE + distance);
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE + distance);
	size_t length = OPENCSTL_NIDX(container, -2 + distance);
	size_t capacity = OPENCSTL_NIDX(container, -3 + distance);
	size_t pos = (*(char**)it - *(char**)container) / type_size;
	if (length + n > capacity + distance) {
		capacity += n;
		void* b = realloc((char*)*container - header_sz + distance*type_size, header_sz + (capacity - distance)* type_size);
		*container = (char*)b + header_sz - distance*type_size;
		OPENCSTL_NIDX(container, -3 + distance) += 2;
	}
	memmove((char*)*container + (pos + n)* type_size, (char*)*container + pos*type_size, (length - pos)*type_size);
	for (size_t i = 0; i < n; i++) {
		memcpy((char*)*container + (pos + i)*type_size, value, type_size);
	}
	OPENCSTL_NIDX(container, -2 + OPENCSTL_NIDX(container, -1) + 1) += n;
}

void __cstl_deque_erase(void** container, void* begin, void* end) {
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE + OPENCSTL_NIDX(container, -1) + 1);
	size_t length = OPENCSTL_NIDX(container, -2 + OPENCSTL_NIDX(container, -1) + 1);
	size_t pos = (*(char**)end - *(char**)begin) / type_size;

	memmove(*(char**)begin, *(char**)end, (char*)*container + (length+1)*type_size - *(char**)end);
	OPENCSTL_NIDX(container, -2 + OPENCSTL_NIDX(container, -1) + 1) -= pos;
}

size_t __cstl_deque_size(void** container) {
	return OPENCSTL_NIDX(container, -2 + OPENCSTL_NIDX(container, -1) + 1);
}

void __cstl_deque_resize(void** container, size_t n, void* value) {
	intmax_t distance = OPENCSTL_NIDX(container, -1) + 1;
	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE + distance);
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE + distance);
	size_t length = OPENCSTL_NIDX(container, -2 + distance);
	size_t capacity = OPENCSTL_NIDX(container, -3 + distance);
	if (capacity + distance < n) {
		capacity = n;
		void* b = calloc(1, header_sz + capacity*type_size);
		memcpy(b, (char*)*container - header_sz + distance*type_size, header_sz);
		memcpy((char*)b + header_sz, *container, length*type_size);
		free((char*)*container - header_sz + distance*type_size);
		*container = (char*)b + header_sz;
		OPENCSTL_NIDX(container, -1) = -1;
		distance = 0;
		OPENCSTL_NIDX(container, -3) = n;
	}
	OPENCSTL_NIDX(container, -2 + distance) = n;
	if (length < n) {
		if (value == NULL) {
			memset((char*)*container + length*type_size, 0, (n - length)*type_size);
		}
		else {
			for (size_t i = length; i < n; i++) {
				memcpy((char*)*container + i*type_size, value, type_size);
			}
		}
	}
}

void* __cstl_deque_begin(void** container) {
	return *container;
}
void* __cstl_deque_end(void** container) {
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE + OPENCSTL_NIDX(container, -1) + 1);
	size_t length = OPENCSTL_NIDX(container, -2 + OPENCSTL_NIDX(container, -1) + 1);
	return (char*)*container + length*type_size;
}
void* __cstl_deque_rbegin(void** container) {
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE + OPENCSTL_NIDX(container, -1) + 1);
	size_t length = OPENCSTL_NIDX(container, -2 + OPENCSTL_NIDX(container, -1) + 1);
	return (char*)*container + (length - 1)*type_size;
}
void* __cstl_deque_rend(void** container) {
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE + OPENCSTL_NIDX(container, -1) + 1);
	return (char*)*container - type_size;
}

void* __cstl_deque_find(void** container, void* iter_begin, void* value) {
	intmax_t distance = OPENCSTL_NIDX(container, -1) + 1;
	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE + distance);
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE + distance);
	size_t length = OPENCSTL_NIDX(container, -2 + distance);
	size_t capacity = OPENCSTL_NIDX(container, -3 + distance);
	size_t pos = (*(char**)iter_begin - *(char**)container) / type_size;
	for (size_t i = pos; i < length; i++) {
		if (memcmp((char*)*container + type_size*(i), value, type_size) == 0) {
			return (char*)*container + type_size*(i);
		}
	}
	return NULL;
}
#endif