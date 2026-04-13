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

#include "tracer.h"
#include "crossplatform.h"
#if defined(OCSTL_CC_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wunused-value"
#pragma clang diagnostic ignored "-Wsign-compare"
#pragma clang diagnostic ignored "-Wpedantic"
#pragma clang diagnostic ignored "-Wgnu-auto-type"
#pragma clang diagnostic ignored "-Wsometimes-uninitialized"
#endif

#if defined(OCSTL_CC_GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wformat"
#endif


#define USE_CSTL_FUNC

// #if defined(__linux__) || defined(__APPLE__)
// #if !defined(__8cc__ )
// #pragma GCC push_options
// #pragma GCC optimize("O0")
// #endif
// #endif
#include "error.h"

#include "deque.h"
#include "vector.h"
#include "list.h"

#include "tree.h"
#include "stack.h"
#include "queue.h"
#include "hashtable.h"

#include "algorithm.h"

#include "compare.h"
#include "random.h"
#include"mt19937.h"
#include "chrono.h"
#include "fstream.h"


#include "sort.h"
#include "logging.h"
#include "version.h"

#define VECTOR(TYPE)            TYPE*
#define LIST(TYPE)              TYPE**
#define SET(TYPE)               TYPE**
#define MAP(TYPE)               TYPE**
#define DEQUE(TYPE)             TYPE*
#define STACK(TYPE)             TYPE*
#define QUEUE(TYPE)             TYPE*
#define UNORDERED_SET(TYPE)     TYPE*
#define UNORDERED_MAP(TYPE)     TYPE*

#if defined(USE_CSTL_FUNC)

#define push_back       cstl_push_back
#define pop_back        cstl_pop_back
#define push_front      cstl_push_front
#define pop_front       cstl_pop_front
#define push            cstl_push
#define pop             cstl_pop
#define insert          cstl_insert
#define erase           cstl_erase
#define resize          cstl_resize
#define assign          cstl_assign
#define find            cstl_find
#define capacity        cstl_capacity
#if defined(OPENCSTL_USE_LENGTH)
#define length          cstl_size
#else
#define size            cstl_size
#endif
#define next            cstl_next
#define prev            cstl_prev
#define begin           cstl_begin
#define end             cstl_end
#define rbegin          cstl_rbegin
#define rend            cstl_rend
#define empty           cstl_empty
#define clear           cstl_clear
#define destroy         cstl_free
#define top             cstl_top
#define front           cstl_front
#define back            cstl_back
#define reserve         cstl_reserve

#define new_deque           cstl_deque
#define new_list            cstl_list
#define new_stack           cstl_stack
#define new_queue           cstl_queue
#define new_unordered_set   cstl_unordered_set
#define new_unordered_map   cstl_unordered_map
#define new_deque           cstl_deque
#define new_vector          cstl_vector
#define new_set             cstl_set
#define new_map             cstl_map
#define new_priority_queue  cstl_priority_queue


#endif


OPENCSTL_FUNC void _cstl_assign(void *container, int argc, ...) {
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
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            if (argc >= 3) {
                cstl_error("Not implemented");
            } else {
                if (argc == 1) {
                    param2 = NULL;
                }
                __cstl_vector_assign((void **) container, *(int *) param1, param2);
            }
        }
        break;
        case OPENCSTL_LIST: {
        }
        break;
        case OPENCSTL_DEQUE: {
            if (argc == 1)param2 = NULL;
            __cstl_deque_assign((void **) container, *(int *) param1, param2);
        }
        break;
        default: cstl_error("Invalid operation");
            break;
    }
    __cstl_va_end(vl);
}

OPENCSTL_FUNC void _cstl_push(void *container, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, container, va_ptr);
#if CSTL_USE_VAARG
    void *value = __cstl_va_arg_next(vl);
#else
    void *value = __cstl_va_arg(va_ptr);
#endif
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_STACK:
        case OPENCSTL_QUEUE: {
            __cstl_deque_push_back((void **) container, value);
        }
        break;
        case OPENCSTL_PRIORITY_QUEUE: {
            __cstl_priority_queue_push((void **) container, value);
        }
        break;
        default: cstl_error("Invalid operator");
            break;
    }
    __cstl_va_end(vl);
}

OPENCSTL_FUNC void _cstl_push_back(void *container, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, container, va_ptr);
#if CSTL_USE_VAARG
    void *param1 = __cstl_va_arg_next(vl);
#else
    void *param1 = __cstl_va_arg(va_ptr);
#endif


    size_t container_type;

    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }

    switch (container_type) {
        case OPENCSTL_VECTOR: {
            __cstl_vector_push_back((void **) container, param1);
        }
        break;
        case OPENCSTL_LIST: {
            __cstl_list_push_back_front((void **) container, param1, -1, 0);
        }
        break;
        case OPENCSTL_DEQUE: {
            __cstl_deque_push_back((void **) container, param1);
        }
        break;
        default: cstl_error("Invalid operator");
            break;
    }

    __cstl_va_end(vl);
}

OPENCSTL_FUNC void _cstl_push_front(void *container, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, container, va_ptr);
#if CSTL_USE_VAARG
    void *param1 = __cstl_va_arg_next(vl);
#else
    void *param1 = __cstl_va_arg(va_ptr);
#endif
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_LIST: {
            __cstl_list_push_back_front((void **) container, param1, 0, -1);
        }
        break;
        case OPENCSTL_DEQUE: {
            __cstl_deque_push_front((void **) container, param1);
        }
        break;
        default: cstl_error("Invalid operator");
            break;
    }
    __cstl_va_end(vl);
}

OPENCSTL_FUNC void _cstl_pop(void *container) {
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_STACK: {
            __cstl_deque_pop_back((void **) container);
        }
        break;
        case OPENCSTL_QUEUE: {
            __cstl_deque_pop_front((void **) container);
        }
        break;
        case OPENCSTL_PRIORITY_QUEUE: {
            __cstl_priority_queue_pop((void **) container);
        }
        break;
        default: cstl_error("Invalid operator");
            break;
    }
}

OPENCSTL_FUNC void _cstl_pop_back(void *container) {
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            __cstl_vector_pop_back((void **) container);
        }
        break;
        case OPENCSTL_LIST: {
            __cstl_list_pop_back_front((void **) container, -1, 0);
        }
        break;
        case OPENCSTL_DEQUE: {
            __cstl_deque_pop_back((void **) container);
        }
        break;
    }
}

OPENCSTL_FUNC void _cstl_pop_front(void *container) {
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_LIST: {
            __cstl_list_pop_back_front((void **) container, 0, -1);
        }
        break;
        case OPENCSTL_DEQUE: {
            __cstl_deque_pop_front((void **) container);
        }
        break;
    }
}

OPENCSTL_FUNC size_type _cstl_size(void *container) {
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    size_type sz = 0;
    switch (container_type) {
        case OPENCSTL_PRIORITY_QUEUE:
        case OPENCSTL_VECTOR: {
            sz = __cstl_vector_size((void **) container);
        }
        break;
        case OPENCSTL_LIST: {
            sz = __cstl_list_size((void **) container);
        }
        break;
        case OPENCSTL_SET:
        case OPENCSTL_MAP: {
            sz = __cstl_tree_size((void **) container);
        }
        break;
        case OPENCSTL_DEQUE: {
            sz = __cstl_deque_size((void **) container);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            sz = __cstl_hashtable_size((void **) container);
        }
        break;
        default: cstl_error("Invalid operation");
            break;
    }
    return sz;
}

OPENCSTL_FUNC size_type _cstl_capacity(void *container) {
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    size_type sz = 0;
    switch (container_type) {
        case OPENCSTL_STACK:
        case OPENCSTL_QUEUE:
        case OPENCSTL_PRIORITY_QUEUE:
        case OPENCSTL_VECTOR: {
            sz = __cstl_vector_capacity((void **) container);
        }
        break;
        case OPENCSTL_DEQUE: {
            sz = __cstl_deque_capacity((void **) container);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            sz = __cstl_hashtable_capacity((void **) container);
        }
        break;
        default: cstl_error("Invalid operation");
            break;
    }
    return sz;
}



OPENCSTL_FUNC void _cstl_insert(void *container, int argc, ...) {
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
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }

    switch (container_type) {
        case OPENCSTL_VECTOR: {
            if (argc == 2)__cstl_vector_insert((void **) container, param1, 1, param2);
            else __cstl_vector_insert((void **) container, param1, *(int *) param2, param3);
        }
        break;
        case OPENCSTL_LIST: {
            if (argc == 2)__cstl_list_insert((void **) container, (void **) param1, 1, param2);
            else __cstl_list_insert((void **) container, (void **) param1, *(int *) param2, param3);
        }
        break;
        case OPENCSTL_DEQUE: {
            if (argc == 2)__cstl_deque_insert((void **) container, param1, 1, param2);
            else __cstl_deque_insert((void **) container, param1, *(int *) param2, param3);
        }
        break;
        case OPENCSTL_MAP: {
            if (argc == 2) __cstl_tree_insert((void **) container, param1, param2);
            else
                cstl_error("Invalid operation");
        }
        break;
        case OPENCSTL_SET: {
            if (argc == 1) __cstl_tree_insert((void **) container, param1,NULL);
            else
                cstl_error("Invalid operation");
        }
        break;
        case OPENCSTL_UNORDERED_MAP: {
            if (argc == 2)__cstl_hashtable_insert((void **) container, param1, param2);
            else
                cstl_error("Invalid operation");
        }
        break;
        case OPENCSTL_UNORDERED_SET: {
            if (argc == 1) {
                __cstl_hashtable_insert((void **) container, param1,NULL);
            } else
                cstl_error("Invalid operation");
        }
        break;
        default: cstl_error("Invalid operation");
            break;
    }

    __cstl_va_end(vl);
}

OPENCSTL_FUNC void _cstl_erase(void *container, int argc, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, argc, va_ptr);
#if CSTL_USE_VAARG
    void *param1 = __cstl_va_arg_next(vl);
    void *param2 = __cstl_va_arg_next(vl);
#else
    void *param1 = __cstl_va_arg(va_ptr);
    void *param2 = __cstl_va_arg((char *) va_ptr + sizeof(void *) * 1);
#endif

    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            if (argc == 1) {
                param2 = (*(char **) param1) + OPENCSTL_NIDX((void**)container, NIDX_TSIZE);
                __cstl_vector_erase((void **) container, param1, &param2);
            } else __cstl_vector_erase((void **) container, param1, param2);
        }
        break;
        case OPENCSTL_LIST: {
            if (argc == 1) {
                param2 = cstl_next(*(void**)param1);
                __cstl_list_erase((void **) container, (void **) param1, (void **) &param2);
            } else __cstl_list_erase((void **) container, (void **) param1, (void **) param2);
        }
        break;
        case OPENCSTL_DEQUE: {
            if (argc == 1) {
                param2 = (*(char **) param1) + OPENCSTL_NIDX((void**)container,
                                                             NIDX_TSIZE + (ptrdiff_t)OPENCSTL_NIDX(((void**)container),
                                                                 -1) + 1);
                __cstl_deque_erase((void **) container, param1, &param2);
            } else __cstl_deque_erase((void **) container, param1, param2);
        }
        break;
        case OPENCSTL_MAP:
        case OPENCSTL_SET: {
            __cstl_tree_erase((void **) container, (void **) *(void **) param1);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            __cstl_hashtable_erase((void **) container, *(void **) param1);
        }
        break;
        default: cstl_error("Invalid operation");
            break;
    }
    __cstl_va_end(vl);
}

OPENCSTL_FUNC void _cstl_resize(void *container, int argc, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, argc, va_ptr);
#if CSTL_USE_VAARG
    void *param1 = __cstl_va_arg_next(vl);
    void *param2 = __cstl_va_arg_next(vl);
#else
    void *param1 = __cstl_va_arg(va_ptr);
    void *param2 = __cstl_va_arg((char *) va_ptr + sizeof(void *) * 1);
#endif

    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            if (argc == 1)param2 = NULL;
            __cstl_vector_resize((void **) container, *(int *) param1, param2);
        }
        break;
        case OPENCSTL_LIST: {
            if (argc == 1)param2 = NULL;
            __cstl_list_resize((void **) container, *(int *) param1, param2);
        }
        break;
        case OPENCSTL_DEQUE: {
            if (argc == 1)param2 = NULL;
            __cstl_deque_resize((void **) container, *(int *) param1, &param2);
        }
        break;
        default: cstl_error("Invalid operation");
            break;
    }
    __cstl_va_end(vl);
}


OPENCSTL_FUNC void _cstl_clear(void *container) {
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            __cstl_vector_clear((void **) container);
        }
        break;
        case OPENCSTL_LIST: {
            __cstl_list_clear((void **) container);
        }
        break;
        case OPENCSTL_STACK:
        case OPENCSTL_QUEUE:
        case OPENCSTL_DEQUE: {
            __cstl_deque_clear((void **) container);
        }
        break;
        case OPENCSTL_SET:
        case OPENCSTL_MAP: {
            __cstl_tree_clear((void **) container);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            __cstl_hashtable_clear((void **) container);
        }
        break;
        default: {
            cstl_error("Invalid operation");
        }
        break;
    }
}

OPENCSTL_FUNC bool _cstl_empty(void *container) {
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    size_t sz = 0;
    switch (container_type) {
        case OPENCSTL_PRIORITY_QUEUE:
        case OPENCSTL_VECTOR: {
            sz = __cstl_vector_size((void **) container);
        }
        break;
        case OPENCSTL_LIST: {
            sz = __cstl_list_size((void **) container);
        }
        break;
        case OPENCSTL_SET:
        case OPENCSTL_MAP: {
            sz = __cstl_tree_size((void **) container);
        }
        break;
        case OPENCSTL_STACK:
        case OPENCSTL_QUEUE:
        case OPENCSTL_DEQUE: {
            sz = __cstl_deque_size((void **) container);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            sz = __cstl_hashtable_size((void **) container);
        }
        break;
        default: {
            cstl_error("Invalid operation");
        };
    }
    return sz ? false : true;
}

OPENCSTL_FUNC void _cstl_free(void *container) {
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            __cstl_vector_free((void **) container);
        }
        break;
        case OPENCSTL_LIST: {
            __cstl_list_free((void **) container);
        }
        break;
        case OPENCSTL_STACK:
        case OPENCSTL_QUEUE:
        case OPENCSTL_DEQUE: {
            __cstl_deque_free((void **) container);
        }
        break;
        case OPENCSTL_SET:
        case OPENCSTL_MAP: {
            __cstl_tree_free((void **) container);
        }
        break;
        case OPENCSTL_PRIORITY_QUEUE: {
            __cstl_vector_free((void **) container);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            __cstl_hashtable_free((void **) container);
        }
        break;
        default: cstl_error("Invalid operation");
            break;
    }
}

OPENCSTL_FUNC void *_cstl_find(void *container, int argc, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, argc, va_ptr);
#if CSTL_USE_VAARG
    void *param1 = __cstl_va_arg_next(vl);
    void *param2 = __cstl_va_arg_next(vl);
#else
    void *param1 = __cstl_va_arg(va_ptr);
    void *param2 = __cstl_va_arg((char *) va_ptr + sizeof(void *) * 1);
#endif


    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            if (argc == 1) return __cstl_vector_find((void **) container, container, param1);
            else return __cstl_vector_find((void **) container, param1, param2);
        }
        break;
        case OPENCSTL_LIST: {
            if (argc == 1)
                return __cstl_list_find((void **) container, (void **) &OPENCSTL_NIDX((void**)container, 0),
                                        param1);
            else return __cstl_list_find((void **) container, (void **) param1, param2);
        }
        break;
        case OPENCSTL_DEQUE: {
            if (argc == 1) return __cstl_deque_find((void **) container, container, param1);
            else return __cstl_deque_find((void **) container, param1, param2);
        }
        break;
        case OPENCSTL_MAP:
        case OPENCSTL_SET: {
            return __cstl_tree_find((void **) container, param1);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            return __cstl_hashtable_find((void **) container, param1);
        }
        break;
        default: cstl_error("Invalid operator");
            break;
    }
    __cstl_va_end(vl);
    return NULL;
}

OPENCSTL_FUNC void _cstl_reserve(void *container, int argc, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, argc, va_ptr);
#if CSTL_USE_VAARG
    void *param1 = __cstl_va_arg_next(vl);
    void *param2 = __cstl_va_arg_next(vl);
#else
    void *param1 = __cstl_va_arg(va_ptr);
    void *param2 = __cstl_va_arg((char *) va_ptr + sizeof(void *) * 1);
#endif

    size_t container_type;
    if (__is_deque((void **) container)) {
        cstl_error("Invalid operation");
    }
    container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);

    switch (container_type) {
        case OPENCSTL_VECTOR: {
            if (argc == 1) {
                __cstl_vector_reserve((void **) container, *(size_t *) param1);
            }
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            if (argc == 1) {
                __cstl_hashtable_reserve((void **) container, *(size_t *) param1);
            }
        }
        break;

        default: {
            cstl_error("Invalid operation");
        }
        break;
    }
    __cstl_va_end(vl);
}


OPENCSTL_FUNC void ___cstl_sort(void *container, int argc, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, argc, va_ptr);
#if CSTL_USE_VAARG
    void *param1 = __cstl_va_arg_next(vl);
    void *param2 = __cstl_va_arg_next(vl);
#else
    void *param1 = __cstl_va_arg(va_ptr);
    void *param2 = __cstl_va_arg((char *) va_ptr + sizeof(void *) * 1);
#endif


    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_LIST: {
            if (argc == 1) {
#if CSTL_USE_VAARG
                __cstl_list_qsort((void **) container, (int (*)(const void *, const void *)) param1);
#else
                __cstl_list_qsort((void **) container, (int (*)(const void *, const void *)) *(void **) param1);
#endif
            }
        }
        break;

        default: cstl_error("Invalid operator");
            break;
    }
    __cstl_va_end(vl);
}

#if defined(__linux__) || defined(__APPLE__)
// #if !defined(__8cc__ )
// #pragma GCC pop_options
// #endif


#if defined(OCSTL_CC_CLANG)
#pragma clang diagnostic pop
#pragma clang diagnostic ignored "-Wgnu-auto-type"
#pragma clang diagnostic ignored "-Wgnu-statement-expression-from-macro-expansion"
#pragma clang diagnostic ignored "-Wvariadic-macro-arguments-omitted"
#endif
#if defined(OCSTL_CC_GCC)
#pragma GCC diagnostic pop
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

#endif
