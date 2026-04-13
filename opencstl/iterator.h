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
#ifndef OPENCSTL_ITERATOR_H
#define OPENCSTL_ITERATOR_H
#include "hashtable.h"
#include "error.h"
#include "vector.h"
#include "list.h"
#include "tree.h"

#define first(IT)                   (*IT)
#define second(IT, TYPE)            ((TYPE)cstl_value(IT, TYPE))


OPENCSTL_FUNC bool __is_hashtable_iter(void *it) {
#ifdef HTM_VEBT
    if (htm == NULL) {
        return false;
    }
    return htm_find(htm, it) != NULL;
#else
    for (size_t i = 0; i < htm_length; i++) {
        if (htm[i].p1 <= it && it < htm[i].p2) {
            return true;
        }
    }
    return false;
#endif
}

OPENCSTL_FUNC void *_cstl_next(void *it) {
    if (__is_hashtable_iter(it)) {
        return __cstl_hashtable_next_prev(it, -1);
    }
    Interval *iv = iveb_find(iveb, it);
    if (iv != NULL) {
        if (iv->ctype == CT_VECTOR || iv->ctype == CT_DEQUE) {
            return __cstl_vector_next(it, iv->type_size);
        }
    }
    size_t node_type = OPENCSTL_NIDX(&it, -3);
    switch (node_type) {
        case OPENCSTL_LIST: {
            return __cstl_list_next_prev(it, -1);
        }
        break;
        case OPENCSTL_SET:
        case OPENCSTL_MAP: {
            return __cstl_tree_next_prev(it, -1, -2, __cstl_tree_toleft);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            return __cstl_hashtable_next_prev(it, -1);
        }
        break;
        default: {
            cstl_error("Invalid operation");
        }
        break;
    }
    return NULL;
}

OPENCSTL_FUNC void *_cstl_prev(void *it) {
    if (__is_hashtable_iter(it)) {
        return __cstl_hashtable_next_prev(it, -2);
    }
    Interval *iv = iveb_find(iveb, it);
    if (iv != NULL) {
        if (iv->ctype == CT_VECTOR || iv->ctype == CT_DEQUE) {
            return __cstl_vector_prev(it, iv->type_size);
        }
    }
    size_t node_type = OPENCSTL_NIDX(&it, -3);
    switch (node_type) {
        case OPENCSTL_LIST: {
            return __cstl_list_next_prev(it, -2);
        }
        break;
        case OPENCSTL_SET:
        case OPENCSTL_MAP: {
            return __cstl_tree_next_prev(it, -2, -1, __cstl_tree_toright);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            return __cstl_hashtable_next_prev(it, -2);
        }
        break;

        default: {
            cstl_error("Invalid operation");
        }
    }
    return NULL;
}

OPENCSTL_FUNC void *_cstl_begin(void *container) {
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            return __cstl_vector_begin((void **) container);
        }
        break;
        case OPENCSTL_LIST: {
            return __cstl_list_begin((void **) container);
        }
        break;
        case OPENCSTL_DEQUE: {
            return __cstl_deque_begin((void **) container);
        }
        break;
        case OPENCSTL_SET:
        case OPENCSTL_MAP: {
            return __cstl_tree_begin((void **) container);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            return __cstl_hashtable_begin((void **) container);
        }
        break;
        default: cstl_error("Invalid operation");
            break;
    }
    return NULL;
}

OPENCSTL_FUNC void *_cstl_rbegin(void *container) {
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            return __cstl_vector_rbegin((void **) container);
        }
        break;
        case OPENCSTL_LIST: {
            return __cstl_list_rbegin((void **) container);
        }
        break;
        case OPENCSTL_DEQUE: {
            return __cstl_deque_rbegin((void **) container);
        }
        break;
        case OPENCSTL_SET:
        case OPENCSTL_MAP: {
            return __cstl_tree_rbegin((void **) container);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            return __cstl_hashtable_rbegin((void **) container);
        }
        break;
        default: cstl_error("Invalid operation");
            break;
    }
    return NULL;
}

OPENCSTL_FUNC void *_cstl_end(void *container) {
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            return __cstl_vector_end((void **) container);
        }
        break;
        case OPENCSTL_LIST: {
            return __cstl_list_end_rend((void **) container);
        }
        break;
        case OPENCSTL_DEQUE: {
            return __cstl_deque_end((void **) container);
        }
        break;
        case OPENCSTL_SET:
        case OPENCSTL_MAP: {
            return __cstl_tree_end_rend((void **) container);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            return __cstl_hashtable_end((void **) container);
        }
        break;
        default: cstl_error("Invalid operation");
            break;
    }
    return NULL;
}

OPENCSTL_FUNC void *_cstl_rend(void *container) {
    size_t container_type;
    if (__is_deque((void **) container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)container), -1) + 1;
        container_type = *(size_t *) ((char *) *(void **) container + NIDX_CTYPE * sizeof(size_t) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            return __cstl_vector_rend((void **) container);
        }
        break;
        case OPENCSTL_LIST: {
            return __cstl_list_end_rend((void **) container);
        }
        break;
        case OPENCSTL_DEQUE: {
            return __cstl_deque_rend((void **) container);
        }
        break;
        case OPENCSTL_SET:
        case OPENCSTL_MAP: {
            return __cstl_tree_end_rend((void **) container);
        }
        break;
        case OPENCSTL_UNORDERED_SET:
        case OPENCSTL_UNORDERED_MAP: {
            return __cstl_hashtable_rend((void **) container);
        }
        break;
        default: cstl_error("Invalid operation");
            break;
    }
    return NULL;
}
#endif //OPENCSTL_ITERATOR_H
