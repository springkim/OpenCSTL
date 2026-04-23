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
//               Copyright (C) 2018-2026, Kim Bomm, Woo Cheol, all rights reserved.
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
#if !defined(_OPENCSTL_QUEUE_H)
#define _OPENCSTL_QUEUE_H

#include"vector.h"
#include"error.h"
#include"salloc.h"


#define cstl_queue(TYPE)	__cstl_queue(sizeof(TYPE),#TYPE)
OPENCSTL_FUNC void *__cstl_queue(size_type64 type_size, char *type) {
    size_type64 header_sz = sizeof(size_type64) * OPENCSTL_HEADER;
    void *ptr = (char *) malloc(header_sz + type_size * 2) + header_sz; // 2 = capacity
    void **container = &ptr;
    OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_QUEUE;
    OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(container, NIDX_TSIZE) = type_size;
    OPENCSTL_NIDX(container, -8) = !strcmp(type, "float");

    OPENCSTL_NIDX(container, -4) = (size_type64) type;
    OPENCSTL_NIDX(container, -3) = 2; //capacity
    OPENCSTL_NIDX(container, -2) = 0; //length
    *container = (char *) ptr + type_size;
    OPENCSTL_NIDX(container, -1) = -type_size - 1; //distance  = -type_size -1
    return ptr;
}

#define _CSTL_PQ_EXPAND(x) x
#define cstl_priority_queue(...)	_CSTL_PQ_EXPAND(_CSTL_PQ_DISPATCH(__VA_ARGS__, NULL, NULL))
#define _CSTL_PQ_DISPATCH(TYPE, COMP, ...) __cstl_priority_queue(sizeof(TYPE),#TYPE,(void*)(COMP))
OPENCSTL_FUNC void *__cstl_priority_queue(size_type64 type_size, char *type, void *compare) {
    size_type64 header_sz = sizeof(size_type64) * OPENCSTL_HEADER;
    void *ptr = (char *) malloc(header_sz + type_size * 1) + header_sz;
    void **container = &ptr;
    OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_PRIORITY_QUEUE;
    OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(container, NIDX_TSIZE) = type_size;
    OPENCSTL_NIDX(container, -8) = !strcmp(type, "float");
    OPENCSTL_NIDX(container, -4) = (size_type64) type;
    OPENCSTL_NIDX(container, -3) = (size_type64) compare; //compare function
    OPENCSTL_NIDX(container, -2) = 1; //capacity
    OPENCSTL_NIDX(container, -1) = 0; //length
    return ptr;
}

OPENCSTL_FUNC void __cstl_priority_queue_push(void **container, void *value) {
    __cstl_vector_push_back(container, value);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    CSTL_COMPARE compare = (CSTL_COMPARE) OPENCSTL_NIDX(container, -3);

    void *tmp = salloc(type_size);
    size_type64 idx = length - 1;
    memcpy(tmp, ((char *) *container) + type_size * idx, type_size);
    while (idx > 0) {
        void *c = ((char *) *container) + type_size * idx;
        void *p = ((char *) *container) + type_size * HEAP_PARENT(idx);
        int r = compare ? compare(tmp, p) : memcmp(tmp, p, type_size);
        if (r <= 0)break;
        memcpy(c, p, type_size);
        idx = HEAP_PARENT(idx);
    }
    memcpy(((char *) *container) + type_size * idx, tmp, type_size);
}

OPENCSTL_FUNC void __cstl_priority_queue_pop(void **container) {
    verify(OPENCSTL_NIDX(container, -1) > 0);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    CSTL_COMPARE compare = (CSTL_COMPARE) OPENCSTL_NIDX(container, -3);
    memcpy(*container, ((char *) *container) + type_size * (length - 1), type_size);
    OPENCSTL_NIDX(container, -1)--;
    length--;
    size_type64 idx = 0;
    void *tmp = salloc(type_size);
    size_type64 L, R, C;
    memcpy(tmp, *container, type_size);
    void *ptr = tmp;
    do {
        L = HEAP_LEFT(idx);
        R = HEAP_RIGHT(idx);
        void *L_node = ((char *) *container) + type_size * L;
        void *R_node = ((char *) *container) + type_size * R;
        int r = compare ? compare(L_node, tmp) : memcmp(L_node, tmp, type_size);
        if (L < length && r > 0) {
            C = L;
            ptr = L_node;
        } else {
            C = idx;
            ptr = tmp;
        }
        r = compare ? compare(R_node, ptr) : memcmp(R_node, ptr, type_size);
        if (R < length && r > 0) {
            C = R;
        }
        if (C == idx)break;
        memcpy(((char *) *container) + type_size * idx, ((char *) *container) + type_size * C, type_size);
        idx = C;
    } while (1);
    memcpy(((char *) *container) + type_size * idx, tmp, type_size);
}


#endif
