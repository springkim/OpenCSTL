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
//               Copyright (C) 2026, Kim Bomm, all rights reserved.
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
#if !defined(_OPENCSTL_C_ALGORITHM_H)
#define _OPENCSTL_C_ALGORITHM_H


#include "defines.h"
#include "compare.h"
#include "iterator.h"
#include "van_emde_boas_tree.h"
#include "types.h"


bool is_even(const void *it) {
    int val = *(int *) it;
    return val % 2 == 0;
}

#define fill(_First, _Last, _Value) _cstl_fill(_First, _Last, &(__typeof__(_Value)){_Value}, sizeof(_Value))
OPENCSTL_FUNC void _cstl_fill(void *_begin, void *_end, void *_value, size_type64 _value_size) {
    void *it = _begin;
    while (it != _end) {
        memcpy(it, _value, _value_size);
        it = cstl_next(it);
    }
}

// ░█████╗░██╗░░░░░░██████╗░░█████╗░██████╗░██╗████████╗██╗░░██╗███╗░░░███╗
// ██╔══██╗██║░░░░░██╔════╝░██╔══██╗██╔══██╗██║╚══██╔══╝██║░░██║████╗░████║
// ███████║██║░░░░░██║░░██╗░██║░░██║██████╔╝██║░░░██║░░░███████║██╔████╔██║
// ██╔══██║██║░░░░░██║░░╚██╗██║░░██║██╔══██╗██║░░░██║░░░██╔══██║██║╚██╔╝██║
// ██║░░██║███████╗╚██████╔╝╚█████╔╝██║░░██║██║░░░██║░░░██║░░██║██║░╚═╝░██║
// ╚═╝░░╚═╝╚══════╝░╚═════╝░░╚════╝░╚═╝░░╚═╝╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░░░░╚═╝


OPENCSTL_FUNC size_type64 _cstl_count(void *container, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, container, va_ptr);
#if CSTL_USE_VAARG
    void *param1 = __cstl_va_arg_next(vl);
#else
    void *param1 = __cstl_va_arg(va_ptr);
#endif
    size_type64 container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_CTYPE * (ptrdiff_t) sizeof(size_type64) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    size_type64 cnt = 0;
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            cnt = __cstl_vector_count((void **) container, param1);
        }
        break;
        case OPENCSTL_DEQUE: {
            cnt = __cstl_deque_count((void **) container, param1);
        }
        break;
        default: {
            yikes("Unsupported container type");
        }
        break;
    }
    __cstl_va_end(vl);
    return cnt;
}

static size_type64 count(void *C, ...) {
    va_list args;
    va_start(args, C);
    size_type64 result = _cstl_count(&C, args);
    va_end(args);
    return result;
}

OPENCSTL_FUNC size_type64 _cstl_count_if(void *container, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, container, va_ptr);
#if CSTL_USE_VAARG
    void *param1 = __cstl_va_arg_next(vl);
#else
    void *param1 = __cstl_va_arg(va_ptr);
#endif
    size_type64 container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_CTYPE * (ptrdiff_t) sizeof(size_type64) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    size_type64 cnt = 0;
#if CSTL_USE_VAARG
    CSTL_COND cond = (CSTL_COND) param1;
#else
    CSTL_COND cond = *(CSTL_COND *) param1;
#endif
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            cnt = __cstl_vector_count_if((void **) container, cond);
        }
        break;
        case OPENCSTL_DEQUE: {
            cnt = __cstl_deque_count_if((void **) container, cond);
        }
        break;
        default: {
            yikes("Unsupported container type");
        }
        break;
    }
    __cstl_va_end(vl);
    return cnt;
}

OPENCSTL_FUNC void *_cstl_lower_bound(void *container, int argc, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, argc, va_ptr);
#if CSTL_USE_VAARG
    void *param1 = __cstl_va_arg_next(vl);
    void *param2 = __cstl_va_arg_next(vl);
    void *param3 = __cstl_va_arg_next(vl);
#else
    void *param1 = __cstl_va_arg(va_ptr);
    void *param2 = __cstl_va_arg((char *) va_ptr + sizeof(void *) * 1);
    void *param3 = __cstl_va_arg((char *) va_ptr + sizeof(void *) * 2);
#endif
    size_type64 container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_CTYPE * (ptrdiff_t) sizeof(size_type64) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    void *ret = NULL;
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            if (argc >= 3) {
                yikes("Not implemented");
            } else {
                if (argc == 1) {
                    size_type64 type_size = OPENCSTL_NIDX((void**)container, NIDX_TSIZE);
                    param2 = _memcmp_funcs[type_size];
                }
                ret = __cstl_vector_lower_bound((void **) container, param1, param2);
            }
        }
        break;
        case OPENCSTL_DEQUE: {
            if (argc >= 3) {
                yikes("Not implemented");
            } else {
                if (argc == 1) {
                    ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
                    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
                    param2 = _memcmp_funcs[type_size];
                }
                ret = __cstl_deque_lower_bound((void **) container, param1, param2);
            }
        }
        break;

        default: yikes("Invalid operation");
            break;
    }
    __cstl_va_end(vl);
    return ret;
}

OPENCSTL_FUNC void *_cstl_upper_bound(void *container, int argc, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, argc, va_ptr);
#if CSTL_USE_VAARG
    void *param1 = __cstl_va_arg_next(vl);
    void *param2 = __cstl_va_arg_next(vl);
    void *param3 = __cstl_va_arg_next(vl);
#else
    void *param1 = __cstl_va_arg(va_ptr);
    void *param2 = __cstl_va_arg((char *) va_ptr + sizeof(void *) * 1);
    void *param3 = __cstl_va_arg((char *) va_ptr + sizeof(void *) * 2);
#endif
    size_type64 container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_CTYPE * (ptrdiff_t) sizeof(size_type64) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    void *ret = NULL;
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            if (argc >= 3) {
                yikes("Not implemented");
            } else {
                if (argc == 1) {
                    size_type64 type_size = OPENCSTL_NIDX((void**)container, NIDX_TSIZE);
                    param2 = _memcmp_funcs[type_size];
                }
                ret = __cstl_vector_upper_bound((void **) container, param1, param2);
            }
        }
        break;
        case OPENCSTL_DEQUE: {
            if (argc >= 3) {
                yikes("Not implemented");
            } else {
                if (argc == 1) {
                    ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
                    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_TSIZE * (ptrdiff_t) sizeof(size_type64) + distance);
                    param2 = _memcmp_funcs[type_size];
                }
                ret = __cstl_deque_upper_bound((void **) container, param1, param2);
            }
        }
        break;

        default: yikes("Invalid operation");
            break;
    }
    __cstl_va_end(vl);
    return ret;
}

// #define __CSTL_LOWER_BOUND_PICK(_1, _2, NAME, ...) NAME
// #define __CSTL_LOWER_BOUND_1(a,b)    cstl_lower_bound((a),(b), NULL)
// #define __CSTL_LOWER_BOUND_2(a,b,c) cstl_lower_bound((a), (b),(c))
// #define lower_bound(...) __CSTL_LOWER_BOUND_PICK(__VA_ARGS__, __CSTL_LOWER_BOUND_2, __CSTL_LOWER_BOUND_1)(__VA_ARGS__)
//
// OPENCSTL_FUNC void cstl_lower_bound(void *_begin, void *_end, _OpenCSTLCompareFunc _cmp) {
// }

// ███╗░░░███╗██╗███╗░░██╗░░░░░░░░░░░██╗░░░░░░░███╗░░░███╗░█████╗░██╗░░██╗
// ████╗░████║██║████╗░██║░░░░░░░░░░██╔╝░░░░░░░████╗░████║██╔══██╗╚██╗██╔╝
// ██╔████╔██║██║██╔██╗██║░░░░░░░░░██╔╝░░░░░░░░██╔████╔██║███████║░╚███╔╝░
// ██║╚██╔╝██║██║██║╚████║░░░░░░░░██╔╝░░░░░░░░░██║╚██╔╝██║██╔══██║░██╔██╗░
// ██║░╚═╝░██║██║██║░╚███║░░░░░░░██╔╝░░░░░░░░░░██║░╚═╝░██║██║░░██║██╔╝╚██╗
// ╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝░░░░░░░╚═╝░░░░░░░░░░░╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝░░╚═╝

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif


#define max_element(C, ...) _cstl_max_element(&(C), ##__VA_ARGS__, NULL)
#define min_element(C, ...) _cstl_min_element(&(C), ##__VA_ARGS__, NULL)

OPENCSTL_FUNC size_type64 __cstl_minmax_container_type(void *container) {
    size_type64 container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) NIDX_CTYPE * (ptrdiff_t) sizeof(size_type64) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_ARRAY:
        case OPENCSTL_VECTOR:
        case OPENCSTL_DEQUE:
        case OPENCSTL_SET:
        case OPENCSTL_MAP:
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP:
            break;
        default:
            yikes("max_element/min_element: unsupported container type");
    }
    return container_type;
}

OPENCSTL_FUNC void *_cstl_max_element(void *container, ...) {
    __cstl_minmax_container_type(container);

    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, container, va_ptr);
#if CSTL_USE_VAARG
    CSTL_COMPARE cmp = (CSTL_COMPARE) __cstl_va_arg_next(vl);
#else
    CSTL_COMPARE cmp = *(CSTL_COMPARE *) __cstl_va_arg(va_ptr);
#endif
    __cstl_va_end(vl);

    void *_begin = _cstl_begin(container);
    void *_end = _cstl_end(container);

    if (_begin == _end) {
        return _end;
    }

    if (cmp == NULL) {
        Interval *tm = iveb_find(iveb, _begin);
        cmp = LESS(tm->type_name);
        if (cmp == NULL) {
            cmp = _memcmp_funcs[tm->type_size];
        }
        verify(cmp != NULL);
    }

    void *max_it = _begin;
    void *it = cstl_next(_begin);
    while (it != _end) {
        if (cmp(it, max_it) > 0) {
            max_it = it;
        }
        it = cstl_next(it);
    }
    return max_it;
}

OPENCSTL_FUNC void *_cstl_min_element(void *container, ...) {
    __cstl_minmax_container_type(container);

    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, container, va_ptr);
#if CSTL_USE_VAARG
    CSTL_COMPARE cmp = (CSTL_COMPARE) __cstl_va_arg_next(vl);
#else
    CSTL_COMPARE cmp = *(CSTL_COMPARE *) __cstl_va_arg(va_ptr);
#endif
    __cstl_va_end(vl);

    void *_begin = _cstl_begin(container);
    void *_end = _cstl_end(container);

    if (_begin == _end) {
        return _end;
    }

    if (cmp == NULL) {
        Interval *tm = iveb_find(iveb, _begin);
        cmp = LESS(tm->type_name);
        if (cmp == NULL) {
            cmp = _memcmp_funcs[tm->type_size];
        }
        verify(cmp != NULL);
    }

    void *min_it = _begin;
    void *it = cstl_next(_begin);
    while (it != _end) {
        if (cmp(it, min_it) < 0) {
            min_it = it;
        }
        it = cstl_next(it);
    }
    return min_it;
}

#endif //_OPENCSTL_C_ALGORITHM_H
