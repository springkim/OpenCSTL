//
// Created by spring on 2026. 4. 12..
//
#pragma once
#if !defined(_OPENCSTL_ZALLOC_H)
#define _OPENCSTL_ZALLOC_H
#include <stdlib.h>
#include "tracer.h"
#ifdef OPENCSTL_TRACER
static void zfree(void *ptr) {
    zerase(ptr);
    free(ptr);
}
#else
#define zfree free
#endif

#ifdef OPENCSTL_TRACER
static inline void *_zalloc(size_t sz) {
    void *ptr = calloc(sz, 1);
    if (ptr) {
        zappend(ptr);
    }
    return ptr;
}

static inline void *zalloc(size_t count, size_t size) {
    return _zalloc(count * size);
}
#else
#define zalloc calloc
#endif

// static void *zmalloc(size_t n) {
//     return _zalloc(n);
// }


//
// #define ZALLOC_GET_MACRO(_1,_2,NAME,...) NAME
// #define zalloc(...) ZALLOC_GET_MACRO(__VA_ARGS__, zcalloc, zmalloc)(__VA_ARGS__)


#ifdef OPENCSTL_TRACER
static void *zrealloc(void *ptr, size_t new_size) {
    if (ptr == NULL) {
        return _zalloc(new_size);
    }

    if (new_size == 0) {
        zfree(ptr);
        return NULL;
    }

    void *new_ptr = realloc(ptr, new_size);
    if (new_ptr == NULL) {
        return NULL;
    }

    if (new_ptr != ptr) {
        zerase(ptr);
        zappend(new_ptr);
    }

    return new_ptr;
}
#else
#define zrealloc realloc
#endif
#endif //_OPENCSTL_ZALLOC_H
