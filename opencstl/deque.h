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
#include "error.h"
#include "zalloc.h"
#include "van_emde_boas_tree.h"
#ifdef _MSC_VER
#pragma warning(disable:4146)
#pragma warning(disable:4308)
#pragma warning(disable:4307)
#endif


OPENCSTL_FUNC ptrdiff_t __is_deque(void **container) {
    if (OPENCSTL_NIDX(container, -1) > INT_MAX)
        return 1;
    return 0;
}

#define cstl_deque(TYPE) __cstl_deque(sizeof(TYPE),#TYPE)
OPENCSTL_FUNC void *__cstl_deque(size_t type_size, char *type) {
    size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
    void *block = zalloc(header_sz + type_size * 2, 1);
    if (block == NULL) {
        cstl_error("Failed to allocate memory for deque");
    }
    void *ptr = (char *) block + header_sz; // 2 = capacity
    void **container = &ptr;

    OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_DEQUE;
    OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(container, NIDX_TSIZE) = type_size;
    OPENCSTL_NIDX(container, -8) = !strcmp(type, "float");

    OPENCSTL_NIDX(container, -4) = (size_t) type;
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

OPENCSTL_FUNC void __cstl_deque_assign(void **container, size_t n, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_t header_sz = *(size_t *) ((char *) *(void **) container + NIDX_HSIZE * sizeof(size_t) + distance);
    size_t type_size = *(size_t *) ((char *) *(void **) container + NIDX_TSIZE * sizeof(size_t) + distance);
    //size_t length = *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance);
    size_t capacity = *(size_t *) ((char *) *(void **) container + -3 * sizeof(size_t) + distance);
    char *type = (char *) *(size_t *) ((char *) *(void **) container + -4 * sizeof(size_t) + distance);
#if !defined(__linux__)
    float valuef = 0.0F;
    if (strcmp(type, "float") == 0) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif

    capacity = n;
    iveb_erase(iveb, (char *) *container + distance);
    void *b = zalloc(1, header_sz + capacity * type_size);
    memcpy(b, (char *) *container - header_sz + distance, header_sz);
    zfree(((char *) *container) - header_sz + distance);
    *container = ((char *) b) + header_sz;
    OPENCSTL_NIDX(container, -1) = -1;
    distance = 0;
    *(size_t *) ((char *) *(void **) container + -3 * sizeof(size_t) + distance) = n;
    *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance) = n;
    iveb_insert(iveb, *container, (char *) (*container) + (type_size * n), CT_DEQUE, type_size, type);

    if (value == NULL) {
        memset((char *) *container, 0, n * type_size);
    } else {
        for (size_t i = 0; i < n; i++) {
            memcpy((char *) *container + type_size * (i), value, type_size);
        }
    }
}

OPENCSTL_FUNC void __cstl_deque_push_back(void **container, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_t header_sz = *(size_t *) ((char *) *(void **) container + NIDX_HSIZE * sizeof(size_t) + distance);
    size_t type_size = *(size_t *) ((char *) *(void **) container + NIDX_TSIZE * sizeof(size_t) + distance);
    size_t length = *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance);
    size_t capacity = *(size_t *) ((char *) *(void **) container + -3 * sizeof(size_t) + distance);
    char *type = (char *) *(size_t *) ((char *) *(void **) container + -4 * sizeof(size_t) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float = *(size_t *) ((char *) *(void **) container + -8 * sizeof(size_t) + distance);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif

    if (length == capacity + distance / (ptrdiff_t) type_size) {
        size_t distance_sz = -distance;
        size_t new_capacity = capacity * 2;
        iveb_erase(iveb, (char *) *container + distance);
        void *b = zalloc(1, header_sz + new_capacity * type_size);
        memcpy(b, (char *) *container - (header_sz + distance_sz), header_sz);
        distance = new_capacity / 4;
        memcpy((char *) b + header_sz + distance * type_size, *container, length * type_size);
        zfree((char *) *container - (header_sz + distance_sz));
        *container = ((char *) b + (header_sz + distance * type_size));
        distance = -distance * type_size;
        *(size_t *) ((char *) *(void **) container + -3 * sizeof(size_t) + distance) *= 2;
        OPENCSTL_NIDX(container, -1) = distance - 1;
        iveb_insert(iveb, (char *) *container + distance,
                    (char *) *container + distance + (type_size * new_capacity), CT_DEQUE, type_size, type);
    }
    memcpy((char *) *container + type_size * length, value, type_size);

    (*(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance))++;
}

OPENCSTL_FUNC void __cstl_deque_push_front(void **container, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_t header_sz = *(size_t *) ((char *) *(void **) container + NIDX_HSIZE * sizeof(size_t) + distance);
    size_t type_size = *(size_t *) ((char *) *(void **) container + NIDX_TSIZE * sizeof(size_t) + distance);
    size_t length = *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance);
    size_t capacity = *(size_t *) ((char *) *(void **) container + -3 * sizeof(size_t) + distance);
    char *type = (char *) *(size_t *) ((char *) *(void **) container + -4 * sizeof(size_t) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float = *(size_t *) ((char *) *(void **) container + -8 * sizeof(size_t) + distance);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    if (distance == 0) {
        size_t new_capacity = capacity * 2;
        iveb_erase(iveb, (char *) *container);
        void *b = zalloc(1, header_sz + new_capacity * type_size);
        memcpy(b, (char *) *container - header_sz, header_sz);
        distance = new_capacity / 4;
        memcpy((char *) b + header_sz + distance * type_size, *container, length * type_size);
        zfree((char *) *container - header_sz);
        *container = ((char *) b + (header_sz + distance * type_size));
        distance = -distance * type_size;
        *(size_t *) ((char *) *(void **) container + -3 * sizeof(size_t) + distance) *= 2;
        iveb_insert(iveb, (char *) *container + distance,
                    (char *) *container + distance + (type_size * new_capacity), CT_DEQUE, type_size, type);
    }
    memcpy((char *) *container - type_size * 2, (char *) *container - type_size, type_size);
    memcpy((char *) *container - type_size, value, type_size);
    *container = (char *) *container - type_size;
    OPENCSTL_NIDX(container, -1) = distance + type_size - 1;
    *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance + type_size) += 1;
}

OPENCSTL_FUNC void __cstl_deque_pop_back(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_t length = *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance);

    if (length <= 0) {
        cstl_error("No elements in cstl_deque");
    }
    *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance) -= 1;
}

OPENCSTL_FUNC void __cstl_deque_pop_front(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_t type_size = *(size_t *) ((char *) *(void **) container + NIDX_TSIZE * sizeof(size_t) + distance);

    if (*(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance - type_size) <= 0) {
        cstl_error("No elements in cstl_deque");
    }
    memcpy(*container, (char *) *container - type_size, type_size);
    *container = (char *) *container + type_size;
    OPENCSTL_NIDX(container, -1) = distance - type_size - 1;
    *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance - type_size) -= 1;
}

OPENCSTL_FUNC void __cstl_deque_insert(void **container, void *it, size_t n, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_t header_sz = *(size_t *) ((char *) *(void **) container + NIDX_HSIZE * sizeof(size_t) + distance);
    size_t type_size = *(size_t *) ((char *) *(void **) container + NIDX_TSIZE * sizeof(size_t) + distance);
    size_t length = *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance);
    size_t capacity = *(size_t *) ((char *) *(void **) container + -3 * sizeof(size_t) + distance);
    char *type = (char *) *(size_t *) ((char *) *(void **) container + -4 * sizeof(size_t) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float = *(size_t *) ((char *) *(void **) container + -8 * sizeof(size_t) + distance);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    size_t pos = (*(char **) it - *(char **) container) / type_size;
    if (length + n > capacity + distance / (ptrdiff_t) type_size) {
        iveb_erase(iveb, (char *) *container + distance);
        capacity += n;
        size_t alloc_sz = header_sz + capacity * type_size - distance;
        void *b = realloc((char *) *container - header_sz + distance, alloc_sz);
        *container = (char *) b + header_sz - distance;
        *(size_t *) ((char *) *(void **) container + -3 * sizeof(size_t) + distance) += n;
        iveb_insert(iveb, (char *) *container + distance,
                    (char *) b + alloc_sz, CT_DEQUE, type_size, type);
    }
    memcpy((char *) *container + (pos + n) * type_size, (char *) *container + pos * type_size,
           (length - pos) * type_size);
    for (size_t i = 0; i < n; i++) {
        memcpy((char *) *container + (pos + i) * type_size, value, type_size);
    }
    *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance) += n;
}

OPENCSTL_FUNC void __cstl_deque_erase(void **container, void *begin, void *end) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_t type_size = *(size_t *) ((char *) *(void **) container + NIDX_TSIZE * sizeof(size_t) + distance);
    size_t length = *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance);
    size_t pos = (*(char **) end - *(char **) begin) / type_size;

    memmove(*(char **) begin, *(char **) end, (char *) *container + (length + 1) * type_size - *(char **) end);
    *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance) -= pos;
}

OPENCSTL_FUNC size_type __cstl_deque_size(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;

    return (size_type) (*(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance));
}

OPENCSTL_FUNC size_type __cstl_deque_capacity(void **container) {
    size_t capacity = OPENCSTL_NIDX(container, -3);

    return (size_type) capacity;
}

OPENCSTL_FUNC void __cstl_deque_resize(void **container, size_t n, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_t header_sz = *(size_t *) ((char *) *(void **) container + NIDX_HSIZE * sizeof(size_t) + distance);
    size_t type_size = *(size_t *) ((char *) *(void **) container + NIDX_TSIZE * sizeof(size_t) + distance);
    size_t length = *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance);
    size_t capacity = *(size_t *) ((char *) *(void **) container + -3 * sizeof(size_t) + distance);
    char *type = (char *) *(size_t *) ((char *) *(void **) container + -4 * sizeof(size_t) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float = *(size_t *) ((char *) *(void **) container + -8 * sizeof(size_t) + distance);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    if (capacity + distance / (ptrdiff_t) type_size < n) {
        capacity = n;
        iveb_erase(iveb, (char *) *container + distance);
        void *b = zalloc(1, header_sz + capacity * type_size);
        memcpy(b, (char *) *container - header_sz + distance, header_sz);
        memcpy((char *) b + header_sz, *container, length * type_size);
        zfree((char *) *container - header_sz + distance);
        *container = (char *) b + header_sz;
        OPENCSTL_NIDX(container, -1) = -1;
        distance = 0;
        *(size_t *) ((char *) *(void **) container + -3 * sizeof(size_t) + distance) = n;
        iveb_insert(iveb, *container, (char *) (*container) + (type_size * n), CT_DEQUE, type_size, type);
    }
    *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance) = n;
    if (length < n) {
        if (*(void **) value == NULL) {
            memset((char *) *container + length * type_size, 0, (n - length) * type_size);
        } else {
            for (size_t i = length; i < n; i++) {
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
    size_t type_size = *(size_t *) ((char *) *(void **) container + NIDX_TSIZE * sizeof(size_t) + distance);
    size_t length = *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance);

    return (char *) *container + length * type_size;
}

OPENCSTL_FUNC void *__cstl_deque_rbegin(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_t type_size = *(size_t *) ((char *) *(void **) container + NIDX_TSIZE * sizeof(size_t) + distance);
    size_t length = *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance);

    return (char *) *container + (length - 1) * type_size;
}

OPENCSTL_FUNC void *__cstl_deque_rend(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_t type_size = *(size_t *) ((char *) *(void **) container + NIDX_TSIZE * sizeof(size_t) + distance);

    return (char *) *container - type_size;
}

OPENCSTL_FUNC void __cstl_deque_clear(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;

    *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance) = 0;
}

OPENCSTL_FUNC void __cstl_deque_free(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_t header_sz = *(size_t *) ((char *) *(void **) container + NIDX_HSIZE * sizeof(size_t) + distance);

    iveb_erase(iveb, (char *) *container + distance);
    zfree((char *) *container - (header_sz - distance));
}

OPENCSTL_FUNC void *__cstl_deque_find(void **container, void *iter_begin, void *value) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    //size_t header_sz = *(size_t *) ((char *) *(void **) container + NIDX_HSIZE * sizeof(size_t) + distance);
    size_t type_size = *(size_t *) ((char *) *(void **) container + NIDX_TSIZE * sizeof(size_t) + distance);
    size_t length = *(size_t *) ((char *) *(void **) container + -2 * sizeof(size_t) + distance);
    //size_t capacity = *(size_t *) ((char *) *(void **) container + -3 * sizeof(size_t) + distance);
    //char *type = (char *) *(size_t *) ((char *) *(void **) container + -4 * sizeof(size_t) + distance);

#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float = *(size_t *) ((char *) *(void **) container + -8 * sizeof(size_t) + distance);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    size_t pos = (*(char **) iter_begin - *(char **) container) / type_size;
    for (size_t i = pos; i < length; i++) {
        if (memcmp((char *) *container + type_size * (i), value, type_size) == 0) {
            return (char *) *container + type_size * (i);
        }
    }
    return NULL;
}
#endif // if !defined(_OPENCSTL_DEQUE_H)
