//
// Created by spring on 4/25/2026.
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

#define cstl_array(TYPE,COUNT)	__cstl_array(sizeof(TYPE),#TYPE,COUNT)
OPENCSTL_FUNC void *__cstl_array(size_type64 type_size, char *type, size_type64 _count) {
    verify(_count !=0);
    size_type64 header_sz = sizeof(size_type64) * OPENCSTL_HEADER;
    void *block = galloc(header_sz + (type_size * _count), type_size);
    if (block == NULL) {
        //yikes("Failed to allocate memory for array");
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
#endif //OPENCSTL_ARRAY_H
