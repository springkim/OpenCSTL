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
//               Copyright (C) 2018-2026, Kim Bomm, all rights reserved.
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
#ifndef OPENCSTL_ARRAY_H
#define OPENCSTL_ARRAY_H
#include"zalloc.h"
#include "van_emde_boas_tree.h"
#include "swap.h"
#include "utility.h"
#include "compare.h"
#include "defines.h"
#include "verify.h"

// ░█████╗░██████╗░██████╗░░█████╗░██╗░░░██╗
// ██╔══██╗██╔══██╗██╔══██╗██╔══██╗╚██╗░██╔╝
// ███████║██████╔╝██████╔╝███████║░╚████╔╝░
// ██╔══██║██╔══██╗██╔══██╗██╔══██║░░╚██╔╝░░
// ██║░░██║██║░░██║██║░░██║██║░░██║░░░██║░░░
// ╚═╝░░╚═╝╚═╝░░╚═╝╚═╝░░╚═╝╚═╝░░╚═╝░░░╚═╝░░░

#define cstl_array(TYPE,COUNT)	__cstl_array(sizeof(TYPE),#TYPE,COUNT)
OPENCSTL_FUNC void *__cstl_array(size_type64 type_size, char *type, size_type64 _count) {
    verify(_count !=0);
    size_type64 header_sz = sizeof(size_type64) * OPENCSTL_HEADER;
    void *block = galloc(header_sz + (type_size * _count), type_size);
    if (block == NULL) {
        fault("Failed to allocate memory for array");
        return NULL;
    }
    void *ptr = ((char *) block) + header_sz;
    void **container = &ptr;
    OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_ARRAY;
    OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(container, NIDX_TSIZE) = type_size;

    OPENCSTL_NIDX(container, -8) = !strcmp(type, "float");

    OPENCSTL_NIDX(container, -4) = (size_type64) type;

    OPENCSTL_NIDX(container, -3) = 0; //
    OPENCSTL_NIDX(container, -2) = _count; //capacity
    OPENCSTL_NIDX(container, -1) = _count; //length
    return ptr;
}


OPENCSTL_FUNC size_type __cstl_array_size(void **container) {
    return (size_type) OPENCSTL_NIDX(container, -1);
}

OPENCSTL_FUNC size_type __cstl_array_capacity(void **container) {
    return (size_type) OPENCSTL_NIDX(container, -2);
}

OPENCSTL_FUNC size_type __cstl_array_max_size(void **container) {
    return (size_type) OPENCSTL_NIDX(container, -2);
}

OPENCSTL_FUNC void *__cstl_array_begin(void **container) {
    return (void *) *container;
}

OPENCSTL_FUNC void *__cstl_array_end(void **container) {
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    return (void *) ((char *) *container + (type_size * length));
}

OPENCSTL_FUNC void *__cstl_array_rbegin(void **container) {
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    return (void *) ((char *) *container + (type_size * (length - 1)));
}

OPENCSTL_FUNC void *__cstl_array_rend(void **container) {
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    return (void *) ((char *) *container - (type_size));
}

OPENCSTL_FUNC void *__cstl_array_find(void **container, void *iter_begin, void *value) {
    //size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    //size_type64 capacity = OPENCSTL_NIDX(container, -2);
    size_type64 pos = (*(char **) iter_begin - *(char **) container) / type_size;
    //char *type = (char *) OPENCSTL_NIDX(container, -4);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = OPENCSTL_NIDX(container, -8);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    for (size_type64 i = pos; i < length; i++) {
        if (memcmp((char *) *container + type_size * (i), value, type_size) == 0) {
            return (char *) *container + type_size * (i);
        }
    }
    return NULL;
}

OPENCSTL_FUNC void __cstl_array_free(void **container) {
#ifdef OPENCSTL_TRACER
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    gfree((char *) (*container) - header_sz);
#else
    gfree(*container);
#endif
}

OPENCSTL_FUNC void *__cstl_array_next(void *it, size_type64 type_size) {
    return (char *) it + type_size;
}

OPENCSTL_FUNC void *__cstl_array_prev(void *it, size_type64 type_size) {
    return (char *) it - type_size;
}

OPENCSTL_FUNC void __cstl_array_reverse(void **container) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    size_type64 capacity = OPENCSTL_NIDX(container, -2);
    char *type = (char *) OPENCSTL_NIDX(container, -4);

    size_type64 idx = 0;
    size_type64 idx_r = length - 1;

    while (idx < idx_r) {
        swap((char *) *container + type_size * (idx), (char *) *container + type_size * (idx_r), type_size);
        idx++;
        idx_r--;
    }
}

// ░█████╗░██╗░░░░░░██████╗░░█████╗░██████╗░██╗████████╗██╗░░██╗███╗░░░███╗
// ██╔══██╗██║░░░░░██╔════╝░██╔══██╗██╔══██╗██║╚══██╔══╝██║░░██║████╗░████║
// ███████║██║░░░░░██║░░██╗░██║░░██║██████╔╝██║░░░██║░░░███████║██╔████╔██║
// ██╔══██║██║░░░░░██║░░╚██╗██║░░██║██╔══██╗██║░░░██║░░░██╔══██║██║╚██╔╝██║
// ██║░░██║███████╗╚██████╔╝╚█████╔╝██║░░██║██║░░░██║░░░██║░░██║██║░╚═╝░██║
// ╚═╝░░╚═╝╚══════╝░╚═════╝░░╚════╝░╚═╝░░╚═╝╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░░░░╚═╝

OPENCSTL_FUNC size_type64 __cstl_array_count(void **container, void *value) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    size_type64 capacity = OPENCSTL_NIDX(container, -2);
    char *type = (char *) OPENCSTL_NIDX(container, -4);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = OPENCSTL_NIDX(container, -8);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    CSTL_EQUALS_FN is_equal = CSTL_EQUALS(type);
    size_type64 cnt = 0;
    for (int i = 0; i < length; i++) {
        void *ptr = ((char *) *container) + (type_size * i);
        if (is_equal(ptr, value, type_size) == 0) {
            cnt++;
        }
    }
    return cnt;
}

OPENCSTL_FUNC size_type64 __cstl_array_count_if(void **container, CSTL_COND cond) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    size_type64 capacity = OPENCSTL_NIDX(container, -2);
    char *type = (char *) OPENCSTL_NIDX(container, -4);


    size_type64 cnt = 0;
    for (int i = 0; i < length; i++) {
        void *ptr = ((char *) *container) + (type_size * i);
        if (cond(ptr)) {
            cnt++;
        }
    }
    return cnt;
}


OPENCSTL_FUNC void *__cstl_array_lower_bound(void **container, void *value, CSTL_COMPARE compare) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    size_type64 capacity = OPENCSTL_NIDX(container, -2);
    char *type = (char *) OPENCSTL_NIDX(container, -4);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = OPENCSTL_NIDX(container, -8);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif

    if (length == 0) return NULL;

    size_type64 L = 0;
    size_type64 R = length;

    while (L < R) {
        size_type64 M = L + (R - L) / 2;
        void *Mptr = ((char *) *container) + (type_size * M);

        if (compare(Mptr, value) < 0)
            L = M + 1;
        else
            R = M;
    }
    if (L >= length) return NULL;
    return ((char *) *container) + (type_size * L);
}

OPENCSTL_FUNC void *__cstl_array_upper_bound(void **container, void *value, CSTL_COMPARE compare) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    size_type64 capacity = OPENCSTL_NIDX(container, -2);
    char *type = (char *) OPENCSTL_NIDX(container, -4);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = OPENCSTL_NIDX(container, -8);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif

    if (length == 0) return NULL;

    size_type64 L = 0;
    size_type64 R = length;

    while (L < R) {
        size_type64 M = L + (R - L) / 2;
        void *Mptr = ((char *) *container) + (type_size * M);

        if (compare(value, Mptr) < 0)
            R = M;
        else
            L = M + 1;
    }
    if (L >= length) return NULL;
    return ((char *) *container) + (type_size * L);
}
#endif //OPENCSTL_ARRAY_H
