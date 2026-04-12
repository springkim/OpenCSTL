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
#if !defined(_OPENCSTL_VECTOR_H)
#define _OPENCSTL_VECTOR_H
#include"error.h"

// ██╗░░░██╗███████╗░█████╗░████████╗░█████╗░██████╗░
// ██║░░░██║██╔════╝██╔══██╗╚══██╔══╝██╔══██╗██╔══██╗
// ╚██╗░██╔╝█████╗░░██║░░╚═╝░░░██║░░░██║░░██║██████╔╝
// ░╚████╔╝░██╔══╝░░██║░░██╗░░░██║░░░██║░░██║██╔══██╗
// ░░╚██╔╝░░███████╗╚█████╔╝░░░██║░░░╚█████╔╝██║░░██║
// ░░░╚═╝░░░╚══════╝░╚════╝░░░░╚═╝░░░░╚════╝░╚═╝░░╚═╝

#define cstl_vector(TYPE)	__cstl_vector(sizeof(TYPE),#TYPE)
OPENCSTL_FUNC void *__cstl_vector(size_t type_size, char *type) {
    size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
    void *block = malloc(header_sz + type_size);
    if (block == NULL) {
        cstl_error("Failed to allocate memory for vector");
    }
    void *ptr = ((char *) block) + header_sz;
    void **container = &ptr;
    OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_VECTOR;
    OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(container, NIDX_TSIZE) = type_size;

    OPENCSTL_NIDX(container, -8) = !strcmp(type, "float");

    OPENCSTL_NIDX(container, -4) = (size_t) type;

    OPENCSTL_NIDX(container, -3) = 0; //
    OPENCSTL_NIDX(container, -2) = 1; //capacity
    OPENCSTL_NIDX(container, -1) = 0; //length
    return ptr;
}

OPENCSTL_FUNC void __cstl_vector_assign(void **container, size_t n, void *value) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t length = OPENCSTL_NIDX(container, -1);
    size_t capacity = OPENCSTL_NIDX(container, -2);
    char *type = (char *) OPENCSTL_NIDX(container, -4);
    size_t is_float = OPENCSTL_NIDX(container, -8);
#if !defined(__linux__) && !defined(__APPLE__)
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    if (capacity < n) {
        void *b = realloc((char *) *container - header_sz, header_sz + n * type_size);
        if (b == NULL) {
            cstl_error("Reallocation failed at vector assign");
        }
        *container = ((char *) b + header_sz);
        OPENCSTL_NIDX(container, -2) = n;
    }
    if (value == NULL) {
        memset(*container, 0, type_size * n);
    } else {
        for (size_t i = 0; i < n; i++) {
            memcpy((char *) *container + type_size * (i), value, type_size);
        }
    }
    OPENCSTL_NIDX(container, -1) = n;
}

OPENCSTL_FUNC void __cstl_vector_push_back(void **container, void *value) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t length = OPENCSTL_NIDX(container, -1);
    size_t capacity = OPENCSTL_NIDX(container, -2);
    char *type = (char *) OPENCSTL_NIDX(container, -4);
    size_t is_float = OPENCSTL_NIDX(container, -8);
#if !defined(__linux__) && !defined(__APPLE__)
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    if (length == capacity) {
        void *b = realloc((char *) *container - header_sz, header_sz + capacity * 2 * type_size);
        if (b == NULL) {
            cstl_error("Reallocation failed at vector push_back");
        }
        *container = ((char *) b + header_sz);
        OPENCSTL_NIDX(container, -2) *= 2;
    }
    memcpy((char *) *container + type_size * length, value, type_size);
    OPENCSTL_NIDX(container, -1)++;
}

OPENCSTL_FUNC void __cstl_vector_pop_back(void **container) {
    if (OPENCSTL_NIDX(container, -1) <= 0) {
        cstl_error("No elements in cstl_vector");
    }
    OPENCSTL_NIDX(container, -1)--;
}

OPENCSTL_FUNC size_type __cstl_vector_size(void **container) {
    return (size_type) OPENCSTL_NIDX(container, -1);
}

OPENCSTL_FUNC size_type __cstl_vector_capacity(void **container) {
    return (size_type) OPENCSTL_NIDX(container, -2);
}

OPENCSTL_FUNC void __cstl_vector_insert(void **container, void *iter, size_t N, void *value) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t length = OPENCSTL_NIDX(container, -1);
    size_t capacity = OPENCSTL_NIDX(container, -2);
    size_t pos = (*(char **) iter - *(char **) container) / type_size;
    char *type = (char *) OPENCSTL_NIDX(container, -4);
    size_t is_float = OPENCSTL_NIDX(container, -8);
#if !defined(__linux__) && !defined(__APPLE__)
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    if (length + N >= capacity) {
        capacity += N;
        void *b = realloc((char *) *container - header_sz, header_sz + capacity * type_size);
        if (b == NULL) {
            cstl_error("Reallocation failed at vector insert");
        }
        *container = ((char *) b + header_sz);
        OPENCSTL_NIDX(container, -2) *= 2;
    }
    memmove((char *) *container + type_size * (pos + N), (char *) *container + type_size * pos,
            (length - pos) * type_size);
    for (size_t i = 0; i < N; i++) {
        memcpy((char *) *container + type_size * (pos + i), value, type_size);
    }
    OPENCSTL_NIDX(container, -1) += N;
}

OPENCSTL_FUNC void __cstl_vector_erase(void **container, void *iter_begin, void *iter_end) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t length = OPENCSTL_NIDX(container, -1);
    size_t capacity = OPENCSTL_NIDX(container, -2);
    size_t pos_begin = (*(char **) iter_begin - *(char **) container) / type_size;
    size_t pos_end = (*(char **) iter_end - *(char **) container) / type_size;
    memmove((char *) *container + type_size * (pos_begin), (char *) *container + type_size * (pos_end),
            (length - pos_end) * type_size);
    OPENCSTL_NIDX(container, -1) -= (pos_end - pos_begin);
}

OPENCSTL_FUNC void *__cstl_vector_begin(void **container) {
    return (void *) *container;
}

OPENCSTL_FUNC void *__cstl_vector_end(void **container) {
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t length = OPENCSTL_NIDX(container, -1);
    return (void *) ((char *) *container + (type_size * length));
}

OPENCSTL_FUNC void *__cstl_vector_rbegin(void **container) {
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t length = OPENCSTL_NIDX(container, -1);
    return (void *) ((char *) *container + (type_size * (length - 1)));
}

OPENCSTL_FUNC void *__cstl_vector_rend(void **container) {
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    return (void *) ((char *) *container - (type_size));
}

OPENCSTL_FUNC void __cstl_vector_resize(void **container, size_t n, void *value) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t length = OPENCSTL_NIDX(container, -1);
    size_t capacity = OPENCSTL_NIDX(container, -2);
    char *type = (char *) OPENCSTL_NIDX(container, -4);
    size_t is_float = OPENCSTL_NIDX(container, -8);
#if !defined(__linux__) && !defined(__APPLE__)
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    if (capacity < n) {
        void *b = realloc((char *) *container - header_sz, header_sz + n * type_size);
        if (b == NULL) {
            cstl_error("Reallocation failed at vector resize");
        }
        *container = ((char *) b + header_sz);
        OPENCSTL_NIDX(container, -2) = n;
    }
    if (n > length) {
        if (value == NULL) {
            memset((char *) *container + type_size * length, 0, type_size * (n - length));
        } else {
            for (size_t i = length; i < n; i++) {
                memcpy((char *) *container + type_size * (i), value, type_size);
            }
        }
    }
    OPENCSTL_NIDX(container, -1) = n;
}

OPENCSTL_FUNC void *__cstl_vector_find(void **container, void *iter_begin, void *value) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t length = OPENCSTL_NIDX(container, -1);
    size_t capacity = OPENCSTL_NIDX(container, -2);
    size_t pos = (*(char **) iter_begin - *(char **) container) / type_size;
    char *type = (char *) OPENCSTL_NIDX(container, -4);
    size_t is_float = OPENCSTL_NIDX(container, -8);
#if !defined(__linux__) && !defined(__APPLE__)
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    for (size_t i = pos; i < length; i++) {
        if (memcmp((char *) *container + type_size * (i), value, type_size) == 0) {
            return (char *) *container + type_size * (i);
        }
    }
    return NULL;
}

OPENCSTL_FUNC void __cstl_vector_clear(void **container) {
    OPENCSTL_NIDX(container, -1) = 0;
}

OPENCSTL_FUNC void __cstl_vector_free(void **container) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    free((char *) (*container) - header_sz);
    *container = NULL;
}

OPENCSTL_FUNC void __cstl_vector_reserve(void **container, size_t n) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t length = OPENCSTL_NIDX(container, -1);
    size_t capacity = OPENCSTL_NIDX(container, -2);
    if (capacity < n) {
        void *b = realloc((char *) *container - header_sz, header_sz + n * type_size);
        if (b == NULL) {
            cstl_error("Reallocation failed at vector reserve");
        }
        *container = ((char *) b + header_sz);
    }
}


#endif
