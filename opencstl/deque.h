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
#if !defined(_OPENCSTL_DEQUE_H)
#define _OPENCSTL_DEQUE_H

#include "zalloc.h"
#include "van_emde_boas_tree.h"
#ifdef _MSC_VER
#pragma warning(disable:4146)
#pragma warning(disable:4308)
#pragma warning(disable:4307)
#endif
#include "defines.h"
#include "types.h"
#include "compare.h"
OPENCSTL_FUNC ptrdiff_t __is_deque(void **container) {
    if (OPENCSTL_NIDX(container, -1) < 0)
        return 1;
    return 0;
}

// Safe container type detection for sort dispatch functions.
// Returns 0 if ptr is not an OpenCSTL container. Sets *distance for deque.
OPENCSTL_FUNC size_type64 __opencstl_container_type(void **container, ptrdiff_t *distance) {
    *distance = 0;
    if (iveb == NULL) return 0;
    Interval *it = iveb_find(iveb, *container);
    if (it == NULL) return 0; // Not an OpenCSTL container
    if (it->ctype == CT_DEQUE) {
        *distance = OPENCSTL_NIDX(container, -1) + 1;
    }
    // return *(_opencstl_ll_ua *) ((char *) *container + (ptrdiff_t)(NIDX_CTYPE) * (ptrdiff_t)sizeof(size_type64) + *distance);
    return *(_opencstl_ll_ua *) ((char *) *container + (ptrdiff_t) NIDX_CTYPE * (ptrdiff_t) sizeof(size_type64) + *distance);
}

#define cstl_deque(TYPE) __cstl_deque(sizeof(TYPE),#TYPE)
OPENCSTL_FUNC void *__cstl_deque(size_type64 type_size, char *type) {
    size_type64 header_sz = sizeof(size_type64) * OPENCSTL_HEADER;
    void *block = calloc(header_sz + type_size * 2, 1);
    verify(block != NULL);
    void *ptr = (char *) block + header_sz; // 2 = capacity
    void **container = &ptr;

    OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_DEQUE;
    OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(container, NIDX_TSIZE) = type_size;
    OPENCSTL_NIDX(container, -8) = !strcmp(type, "float");

    OPENCSTL_NIDX(container, -4) = (size_type64) type;
    OPENCSTL_NIDX(container, -3) = 2; // capacity
    OPENCSTL_NIDX(container, -2) = 0; // length
    void *buf_start = ptr;
    *container = (char *) ptr + type_size;
    OPENCSTL_NIDX(container, -1) = -type_size - 1;
    bool iveb_init = false;
    if (iveb == NULL) {
        iveb = iveb_new();
        iveb_init = true;
    }
    iveb_insert(iveb, buf_start, (char *) buf_start + (type_size * 2), CT_DEQUE, type_size, type);
    if (iveb_init) {
        atexit(__opencstl_iveb_destroy);
    }
    return ptr;
}

OPENCSTL_FUNC void __cstl_deque_assign(void **container, size_type64 n, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 header_sz = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_HSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    //size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t)(-2) * (ptrdiff_t)sizeof(size_type64) + distance);
    size_type64 capacity = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-3) * (ptrdiff_t) sizeof(size_type64) + distance);
    char *type = (char *) *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-4) * (ptrdiff_t) sizeof(size_type64) + distance);
#if !defined(__linux__)
    float valuef = 0.0F;
    if (strcmp(type, "float") == 0) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif

    capacity = n;
    iveb_erase(iveb, (char *) *container + distance);
    void *b = calloc(1, header_sz + capacity * type_size);
    memcpy(b, (char *) *container - header_sz + distance, header_sz);
    free(((char *) *container) - header_sz + distance);
    *container = ((char *) b) + header_sz;
    OPENCSTL_NIDX(container, -1) = -1;
    distance = 0;
    *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-3) * (ptrdiff_t) sizeof(size_type64) + distance) = n;
    *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance) = n;
    iveb_insert(iveb, *container, (char *) (*container) + (type_size * n), CT_DEQUE, type_size, type);

    if (value == NULL) {
        memset((char *) *container, 0, n * type_size);
    } else {
        for (size_type64 i = 0; i < n; i++) {
            memcpy((char *) *container + type_size * (i), value, type_size);
        }
    }
}

OPENCSTL_FUNC void __cstl_deque_push_back(void **container, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 header_sz = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_HSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 capacity = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-3) * (ptrdiff_t) sizeof(size_type64) + distance);
    char *type = (char *) *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-4) * (ptrdiff_t) sizeof(size_type64) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-8) * (ptrdiff_t) sizeof(size_type64) + distance);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif

    if (length == capacity + distance / (ptrdiff_t) type_size) {
        size_type64 distance_sz = -distance;
        size_type64 new_capacity = capacity * 2;
        iveb_erase(iveb, (char *) *container + distance);
        void *b = calloc(1, header_sz + new_capacity * type_size);
        memcpy(b, (char *) *container - (header_sz + distance_sz), header_sz);
        distance = new_capacity / 4;
        memcpy((char *) b + header_sz + distance * type_size, *container, length * type_size);
        free((char *) *container - (header_sz + distance_sz));
        *container = ((char *) b + (header_sz + distance * type_size));
        distance = -distance * type_size;
        *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-3) * (ptrdiff_t) sizeof(size_type64) + distance) *= 2;
        OPENCSTL_NIDX(container, -1) = distance - 1;
        iveb_insert(iveb, (char *) *container + distance,
                    (char *) *container + distance + (type_size * new_capacity), CT_DEQUE, type_size, type);
    }
    memcpy((char *) *container + type_size * length, value, type_size);

    (*(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance))++;
}

OPENCSTL_FUNC void __cstl_deque_push_front(void **container, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 header_sz = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_HSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 capacity = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-3) * (ptrdiff_t) sizeof(size_type64) + distance);
    char *type = (char *) *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-4) * (ptrdiff_t) sizeof(size_type64) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-8) * (ptrdiff_t) sizeof(size_type64) + distance);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    if (distance == 0) {
        size_type64 new_capacity = capacity * 2;
        iveb_erase(iveb, (char *) *container);
        void *b = calloc(1, header_sz + new_capacity * type_size);
        memcpy(b, (char *) *container - header_sz, header_sz);
        distance = new_capacity / 4;
        memcpy((char *) b + header_sz + distance * type_size, *container, length * type_size);
        free((char *) *container - header_sz);
        *container = ((char *) b + (header_sz + distance * type_size));
        distance = -distance * type_size;
        *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-3) * (ptrdiff_t) sizeof(size_type64) + distance) *= 2;
        iveb_insert(iveb, (char *) *container + distance,
                    (char *) *container + distance + (type_size * new_capacity), CT_DEQUE, type_size, type);
    }
    memcpy((char *) *container - type_size * 2, (char *) *container - type_size, type_size);
    memcpy((char *) *container - type_size, value, type_size);
    *container = (char *) *container - type_size;
    OPENCSTL_NIDX(container, -1) = distance + type_size - 1;
    *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance + type_size) += 1;
}

OPENCSTL_FUNC void __cstl_deque_pop_back(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);

    verify(length > 0);
    *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance) -= 1;
}

OPENCSTL_FUNC void __cstl_deque_pop_front(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);

    verify(*(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t)(-2) * (ptrdiff_t)sizeof(size_type64) + distance - type_size) > 0);
    memcpy(*container, (char *) *container - type_size, type_size);
    *container = (char *) *container + type_size;
    OPENCSTL_NIDX(container, -1) = distance - type_size - 1;
    *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance - type_size) -= 1;
}

OPENCSTL_FUNC void __cstl_deque_insert(void **container, void *it, size_type64 n, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 header_sz = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_HSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 capacity = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-3) * (ptrdiff_t) sizeof(size_type64) + distance);
    char *type = (char *) *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-4) * (ptrdiff_t) sizeof(size_type64) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-8) * (ptrdiff_t) sizeof(size_type64) + distance);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    size_type64 pos = (*(char **) it - *(char **) container) / type_size;
    if (length + n > capacity + distance / (ptrdiff_t) type_size) {
        iveb_erase(iveb, (char *) *container + distance);
        capacity += n;
        size_type64 alloc_sz = header_sz + capacity * type_size - distance;
        void *b = realloc((char *) *container - header_sz + distance, alloc_sz);
        *container = (char *) b + header_sz - distance;
        *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-3) * (ptrdiff_t) sizeof(size_type64) + distance) += n;
        iveb_insert(iveb, (char *) *container + distance,
                    (char *) b + alloc_sz, CT_DEQUE, type_size, type);
    }
    memcpy((char *) *container + (pos + n) * type_size, (char *) *container + pos * type_size,
           (length - pos) * type_size);
    for (size_type64 i = 0; i < n; i++) {
        memcpy((char *) *container + (pos + i) * type_size, value, type_size);
    }
    *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance) += n;
}

OPENCSTL_FUNC void __cstl_deque_erase(void **container, void *begin, void *end) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 pos = (*(char **) end - *(char **) begin) / type_size;

    memmove(*(char **) begin, *(char **) end, (char *) *container + (length + 1) * type_size - *(char **) end);
    *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance) -= pos;
}

OPENCSTL_FUNC size_type __cstl_deque_size(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;

    return (size_type) (*(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance));
}

OPENCSTL_FUNC size_type __cstl_deque_capacity(void **container) {
    size_type64 capacity = OPENCSTL_NIDX(container, -3);

    return (size_type) capacity;
}

OPENCSTL_FUNC void __cstl_deque_resize(void **container, size_type64 n, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 header_sz = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_HSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 capacity = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-3) * (ptrdiff_t) sizeof(size_type64) + distance);
    char *type = (char *) *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-4) * (ptrdiff_t) sizeof(size_type64) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-8) * (ptrdiff_t) sizeof(size_type64) + distance);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    if (capacity + distance / (ptrdiff_t) type_size < n) {
        capacity = n;
        iveb_erase(iveb, (char *) *container + distance);
        void *b = calloc(1, header_sz + capacity * type_size);
        memcpy(b, (char *) *container - header_sz + distance, header_sz);
        memcpy((char *) b + header_sz, *container, length * type_size);
        free((char *) *container - header_sz + distance);
        *container = (char *) b + header_sz;
        OPENCSTL_NIDX(container, -1) = -1;
        distance = 0;
        *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-3) * (ptrdiff_t) sizeof(size_type64) + distance) = n;
        iveb_insert(iveb, *container, (char *) (*container) + (type_size * n), CT_DEQUE, type_size, type);
    }
    *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance) = n;
    if (length < n) {
        if (*(void **) value == NULL) {
            memset((char *) *container + length * type_size, 0, (n - length) * type_size);
        } else {
            for (size_type64 i = length; i < n; i++) {
                memcpy((char *) *container + i * type_size, value, type_size);
            }
        }
    }
}

OPENCSTL_FUNC void *__cstl_deque_begin(void **container) {
    return *container;
}

OPENCSTL_FUNC void *__cstl_deque_end(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);

    return (char *) *container + length * type_size;
}

OPENCSTL_FUNC void *__cstl_deque_rbegin(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);

    return (char *) *container + (length - 1) * type_size;
}

OPENCSTL_FUNC void *__cstl_deque_rend(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);

    return (char *) *container - type_size;
}

OPENCSTL_FUNC void __cstl_deque_clear(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;

    *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance) = 0;
}

OPENCSTL_FUNC void __cstl_deque_free(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 header_sz = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_HSIZE * (ptrdiff_t) sizeof(size_type64) + distance);

    iveb_erase(iveb, (char *) *container + distance);
    free((char *) *container - (header_sz - distance));
}

OPENCSTL_FUNC void *__cstl_deque_find(void **container, void *iter_begin, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    //size_type64 header_sz = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t)NIDX_HSIZE * (ptrdiff_t)sizeof(size_type64) + distance);
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);
    //size_type64 capacity = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t)(-3) * (ptrdiff_t)sizeof(size_type64) + distance);
    //char *type = (char *) *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t)(-4) * (ptrdiff_t)sizeof(size_type64) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-8) * (ptrdiff_t) sizeof(size_type64) + distance);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    size_type64 pos = (*(char **) iter_begin - *(char **) container) / type_size;
    for (size_type64 i = pos; i < length; i++) {
        if (memcmp((char *) *container + type_size * (i), value, type_size) == 0) {
            return (char *) *container + type_size * (i);
        }
    }
    return NULL;
}

OPENCSTL_FUNC size_type __cstl_deque_max_size(void **container) {
    return INT_MAX;
}

OPENCSTL_FUNC void __cstl_deque_shrink_to_fit(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 header_sz = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_HSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 capacity = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-3) * (ptrdiff_t) sizeof(size_type64) + distance);
    char *type = (char *) *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-4) * (ptrdiff_t) sizeof(size_type64) + distance);

    // 이미 꽉 차있고 앞쪽 여유도 없으면 아무것도 안 함
    if (length == capacity && distance == 0) {
        return;
    }

    size_type64 new_capacity = length > 0 ? length : 1;

    iveb_erase(iveb, (char *) *container + distance);
    void *b = calloc(1, header_sz + new_capacity * type_size);
    verify(b != NULL);
    // 헤더 복사
    memcpy(b, (char *) *container - header_sz + distance, header_sz);
    // 원소들을 새 버퍼의 맨 앞(distance=0)으로 복사
    memcpy((char *) b + header_sz, *container, length * type_size);
    free((char *) *container - header_sz + distance);

    *container = (char *) b + header_sz;
    OPENCSTL_NIDX(container, -1) = -1; // distance = 0
    OPENCSTL_NIDX(container, -3) = new_capacity;
    OPENCSTL_NIDX(container, -2) = length;

    iveb_insert(iveb, *container,
                (char *) *container + (type_size * new_capacity),
                CT_DEQUE, type_size, type);
}

OPENCSTL_FUNC void __cstl_deque_reverse(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);

    if (length < 2) {
        return;
    }

    // 작은 타입은 스택 버퍼, 큰 타입은 heap (list_swap_data와 동일 패턴)
    unsigned char stackbuf[128];
    unsigned char *tmp = (type_size <= sizeof(stackbuf))
                             ? stackbuf
                             : (unsigned char *) calloc(type_size, 1);
    verify(tmp != NULL);

    char *base = (char *) *container;
    for (size_type64 i = 0, j = length - 1; i < j; i++, j--) {
        memcpy(tmp, base + i * type_size, type_size);
        memcpy(base + i * type_size, base + j * type_size, type_size);
        memcpy(base + j * type_size, tmp, type_size);
    }

    if (tmp != stackbuf) {
        free(tmp);
    }
}

// ░█████╗░██╗░░░░░░██████╗░░█████╗░██████╗░██╗████████╗██╗░░██╗███╗░░░███╗
// ██╔══██╗██║░░░░░██╔════╝░██╔══██╗██╔══██╗██║╚══██╔══╝██║░░██║████╗░████║
// ███████║██║░░░░░██║░░██╗░██║░░██║██████╔╝██║░░░██║░░░███████║██╔████╔██║
// ██╔══██║██║░░░░░██║░░╚██╗██║░░██║██╔══██╗██║░░░██║░░░██╔══██║██║╚██╔╝██║
// ██║░░██║███████╗╚██████╔╝╚█████╔╝██║░░██║██║░░░██║░░░██║░░██║██║░╚═╝░██║
// ╚═╝░░╚═╝╚══════╝░╚═════╝░░╚════╝░╚═╝░░╚═╝╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░░░░╚═╝

OPENCSTL_FUNC size_type64 __cstl_deque_count(void **container, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);
    char *type = (char *) *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-4) * (ptrdiff_t) sizeof(size_type64) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-8) * (ptrdiff_t) sizeof(size_type64) + distance);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    CSTL_EQUALS_FN is_equal = CSTL_EQUALS(type);
    size_type64 cnt = 0;
    for (size_type64 i = 0; i < length; i++) {
        void *ptr = (char *) *container + (type_size * i);
        if (is_equal(ptr, value, type_size) == 0) {
            cnt++;
        }
    }
    return cnt;
}

OPENCSTL_FUNC size_type64 __cstl_deque_count_if(void **container, CSTL_COND cond) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);

    size_type64 cnt = 0;
    for (size_type64 i = 0; i < length; i++) {
        void *ptr = (char *) *container + (type_size * i);
        if (cond(ptr)) {
            cnt++;
        }
    }
    return cnt;
}

OPENCSTL_FUNC void *__cstl_deque_lower_bound(void **container, void *value, CSTL_COMPARE compare) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-8) * (ptrdiff_t) sizeof(size_type64) + distance);
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

OPENCSTL_FUNC void *__cstl_deque_upper_bound(void **container, void *value, CSTL_COMPARE compare) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-8) * (ptrdiff_t) sizeof(size_type64) + distance);
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
#endif // if !defined(_OPENCSTL_DEQUE_H)
