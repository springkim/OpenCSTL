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
// or
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
