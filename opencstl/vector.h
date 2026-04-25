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
#include"zalloc.h"
#include "van_emde_boas_tree.h"
#include "swap.h"
#include "utility.h"
#include "compare.h"
// ██╗░░░██╗███████╗░█████╗░████████╗░█████╗░██████╗░
// ██║░░░██║██╔════╝██╔══██╗╚══██╔══╝██╔══██╗██╔══██╗
// ╚██╗░██╔╝█████╗░░██║░░╚═╝░░░██║░░░██║░░██║██████╔╝
// ░╚████╔╝░██╔══╝░░██║░░██╗░░░██║░░░██║░░██║██╔══██╗
// ░░╚██╔╝░░███████╗╚█████╔╝░░░██║░░░╚█████╔╝██║░░██║
// ░░░╚═╝░░░╚══════╝░╚════╝░░░░╚═╝░░░░╚════╝░╚═╝░░╚═╝

#define cstl_vector(TYPE)	__cstl_vector(sizeof(TYPE),#TYPE)
OPENCSTL_FUNC void *__cstl_vector(size_type64 type_size, char *type) {
    size_type64 header_sz = sizeof(size_type64) * OPENCSTL_HEADER;
    void *block = calloc(header_sz + type_size, 1);
    if (block == NULL) {
        yikes("Failed to allocate memory for vector");
    }
    void *ptr = ((char *) block) + header_sz;
    void **container = &ptr;
    OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_VECTOR;
    OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(container, NIDX_TSIZE) = type_size;

    OPENCSTL_NIDX(container, -8) = !strcmp(type, "float");

    OPENCSTL_NIDX(container, -4) = (size_type64) type;

    OPENCSTL_NIDX(container, -3) = 0; //
    OPENCSTL_NIDX(container, -2) = 1; //capacity
    OPENCSTL_NIDX(container, -1) = 0; //length
    bool iveb_init = false;
    if (iveb == NULL) {
        iveb = iveb_new();
        iveb_init = true;
    }
    iveb_insert(iveb, ptr, (char *) ptr + (type_size), CT_VECTOR, type_size, type);
    if (iveb_init) {
        atexit(__opencstl_iveb_destroy);
    }
    return ptr;
}

OPENCSTL_FUNC void __cstl_vector_assign(void **container, size_type64 n, void *value) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    //size_type64 length = OPENCSTL_NIDX(container, -1);
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
    if (capacity < n) {
        iveb_erase(iveb, *container);
        void *b = realloc((char *) *container - header_sz, header_sz + n * type_size);
        if (b == NULL) {
            yikes("Reallocation failed at vector assign");
        }
        *container = ((char *) b + header_sz);
        OPENCSTL_NIDX(container, -2) = n;
        iveb_insert(iveb, *container, (char *) (*container) + (type_size * n), CT_VECTOR, type_size, type);
    }
    if (value == NULL) {
        memset(*container, 0, type_size * n);
    } else {
        for (size_type64 i = 0; i < n; i++) {
            memcpy((char *) *container + type_size * (i), value, type_size);
        }
    }
    OPENCSTL_NIDX(container, -1) = n;
}

OPENCSTL_FUNC void __cstl_vector_push_back(void **container, void *value) {
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
    if (length == capacity) {
        iveb_erase(iveb, *container);
        size_type64 new_capaciy = get_new_capacity(capacity);
        void *b = realloc((char *) *container - header_sz, header_sz + new_capaciy * type_size);
        if (b == NULL) {
            yikes("Reallocation failed at vector push_back");
        }
        *container = ((char *) b + header_sz);
        OPENCSTL_NIDX(container, -2) = new_capaciy;
        iveb_insert(iveb, *container, (char *) (*container) + (type_size * new_capaciy), CT_VECTOR, type_size, type);
    }
    memcpy((char *) *container + type_size * length, value, type_size);
    OPENCSTL_NIDX(container, -1)++;
}

OPENCSTL_FUNC void __cstl_vector_pop_back(void **container) {
    if (OPENCSTL_NIDX(container, -1) <= 0) {
        yikes("No elements in cstl_vector");
    }
    OPENCSTL_NIDX(container, -1)--;
}

OPENCSTL_FUNC size_type __cstl_vector_size(void **container) {
    return (size_type) OPENCSTL_NIDX(container, -1);
}

OPENCSTL_FUNC size_type __cstl_vector_capacity(void **container) {
    return (size_type) OPENCSTL_NIDX(container, -2);
}

OPENCSTL_FUNC void __cstl_vector_insert(void **container, void *iter, size_type64 N, void *value) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    size_type64 capacity = OPENCSTL_NIDX(container, -2);
    size_type64 pos = (*(char **) iter - *(char **) container) / type_size;
    char *type = (char *) OPENCSTL_NIDX(container, -4);

#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float = OPENCSTL_NIDX(container, -8);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    if (length + N >= capacity) {
        iveb_erase(iveb, *container);
        size_type64 new_capaciy = get_new_capacity(capacity + N);
        void *b = realloc((char *) *container - header_sz, header_sz + new_capaciy * type_size);
        if (b == NULL) {
            yikes("Reallocation failed at vector insert");
        }
        *container = ((char *) b + header_sz);
        OPENCSTL_NIDX(container, -2) = new_capaciy;
        iveb_insert(iveb, *container, (char *) (*container) + (type_size * new_capaciy), CT_VECTOR, type_size, type);
    }
    memmove((char *) *container + type_size * (pos + N), (char *) *container + type_size * pos,
            (length - pos) * type_size);
    for (size_type64 i = 0; i < N; i++) {
        memcpy((char *) *container + type_size * (pos + i), value, type_size);
    }
    OPENCSTL_NIDX(container, -1) += N;
}

OPENCSTL_FUNC void __cstl_vector_erase(void **container, void *iter_begin, void *iter_end) {
    //size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    //size_type64 capacity = OPENCSTL_NIDX(container, -2);
    size_type64 pos_begin = (*(char **) iter_begin - *(char **) container) / type_size;
    size_type64 pos_end = (*(char **) iter_end - *(char **) container) / type_size;
    memmove((char *) *container + type_size * (pos_begin), (char *) *container + type_size * (pos_end),
            (length - pos_end) * type_size);
    OPENCSTL_NIDX(container, -1) -= (pos_end - pos_begin);
}

OPENCSTL_FUNC void *__cstl_vector_begin(void **container) {
    return (void *) *container;
}

OPENCSTL_FUNC void *__cstl_vector_end(void **container) {
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    return (void *) ((char *) *container + (type_size * length));
}

OPENCSTL_FUNC void *__cstl_vector_rbegin(void **container) {
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    return (void *) ((char *) *container + (type_size * (length - 1)));
}

OPENCSTL_FUNC void *__cstl_vector_rend(void **container) {
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    return (void *) ((char *) *container - (type_size));
}

OPENCSTL_FUNC void __cstl_vector_resize(void **container, size_type64 n, void *value) {
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
    if (capacity < n) {
        iveb_erase(iveb, *container);
        void *b = realloc((char *) *container - header_sz, header_sz + n * type_size);
        if (b == NULL) {
            yikes("Reallocation failed at vector resize");
        }
        *container = ((char *) b + header_sz);
        OPENCSTL_NIDX(container, -2) = n;
        iveb_insert(iveb, *container, (char *) (*container) + (type_size * n), CT_VECTOR, type_size, type);
    }
    if (n > length) {
        if (value == NULL) {
            memset((char *) *container + type_size * length, 0, type_size * (n - length));
        } else {
            for (size_type64 i = length; i < n; i++) {
                memcpy((char *) *container + type_size * (i), value, type_size);
            }
        }
    }
    OPENCSTL_NIDX(container, -1) = n;
}

OPENCSTL_FUNC void *__cstl_vector_find(void **container, void *iter_begin, void *value) {
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

OPENCSTL_FUNC void __cstl_vector_clear(void **container) {
    OPENCSTL_NIDX(container, -1) = 0;
}

OPENCSTL_FUNC void __cstl_vector_free(void **container) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    iveb_erase(iveb, *container);
    free((char *) (*container) - header_sz);
    *container = NULL;
}

OPENCSTL_FUNC void __cstl_vector_reserve(void **container, size_type64 n) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    //size_type64 length = OPENCSTL_NIDX(container, -1);
    size_type64 capacity = OPENCSTL_NIDX(container, -2);
    char *type = (char *) OPENCSTL_NIDX(container, -4);
    if (capacity < n) {
        iveb_erase(iveb, *container);
        void *b = realloc((char *) *container - header_sz, header_sz + n * type_size);
        if (b == NULL) {
            yikes("Reallocation failed at vector reserve");
        }
        *container = ((char *) b + header_sz);
        OPENCSTL_NIDX(container, -2) = n;
        iveb_insert(iveb, *container, (char *) (*container) + (type_size * n), CT_VECTOR, type_size, type);
    }
}

OPENCSTL_FUNC void *__cstl_vector_next(void *it, size_type64 type_size) {
    return (char *) it + type_size;
}

OPENCSTL_FUNC void *__cstl_vector_prev(void *it, size_type64 type_size) {
    return (char *) it - type_size;
}

OPENCSTL_FUNC size_type __cstl_vector_max_size(void **container) {
    return INT_MAX;
}


OPENCSTL_FUNC void __cstl_vector_shrink_to_fit(void **container) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    size_type64 capacity = OPENCSTL_NIDX(container, -2);
    char *type = (char *) OPENCSTL_NIDX(container, -4);

    if (capacity == length) {
        return;
    }
    size_type64 new_capacity = length > 0 ? length : 1;

    iveb_erase(iveb, *container);
    void *b = realloc((char *) *container - header_sz, header_sz + new_capacity * type_size);
    if (b == NULL) {
        yikes("Reallocation failed at vector shrink_to_fit");
    }
    *container = ((char *) b + header_sz);
    OPENCSTL_NIDX(container, -2) = new_capacity;
    iveb_insert(iveb, *container, (char *) (*container) + (type_size * new_capacity), CT_VECTOR, type_size, type);
}

OPENCSTL_FUNC void __cstl_vector_reverse(void **container) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);
    size_type64 capacity = OPENCSTL_NIDX(container, -2);
    char *type = (char *) OPENCSTL_NIDX(container, -4);

    int idx = 0;
    int idx_r = length - 1;

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


OPENCSTL_FUNC size_type64 __cstl_vector_count(void **container, void *value) {
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

OPENCSTL_FUNC size_type64 __cstl_vector_count_if(void **container, CSTL_COND cond) {
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


OPENCSTL_FUNC void *__cstl_vector_lower_bound(void **container, void *value, CSTL_COMPARE compare) {
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

OPENCSTL_FUNC void *__cstl_vector_upper_bound(void **container, void *value, CSTL_COMPARE compare) {
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
#endif
