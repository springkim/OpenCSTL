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
typedef void (*free_fn)(void *ptr);

typedef void *(*malloc_fn)(size_t sz);

typedef void *(*realloc_fn)(void *ptr, size_t new_size);

typedef void * (*calloc_fn)(size_t cnt, size_t sz);

static free_fn ofree = free;
static malloc_fn omalloc = malloc;
static realloc_fn orealloc = realloc;
static calloc_fn ocalloc = calloc;

static void zfree(void *ptr) {
    zremove(ptr);
    ofree(ptr);
}


static void *_zcalloc(size_type64 cnt, size_type64 sz, char *file, const char *func, int line) {
    void *ptr = ocalloc(cnt, sz);
    if (ptr) {
        zinsert(ptr, file, func, line);
    }
    return ptr;
}


static void *_zmalloc(size_type64 sz, char *file, const char *func, int line) {
    void *ptr = omalloc(sz);
    if (ptr) {
        zinsert(ptr, file, func, line);
    }
    return ptr;
}


static void *_zrealloc(void *ptr, size_type64 new_size, char *file, const char *func, int line) {
    if (ptr == NULL) {
        return _zmalloc(new_size, file, func, line);
    }

    if (new_size == 0) {
        zfree(ptr);
        return NULL;
    }

    void *new_ptr = orealloc(ptr, new_size);
    if (new_ptr == NULL) {
        return NULL;
    }

    if (new_ptr != ptr) {
        zremove(ptr);
        zinsert(new_ptr, file, func, line);
    }

    return new_ptr;
}


#define free(ptr) zfree(ptr)
#define calloc(cnt, sz) _zcalloc(cnt, sz,__FILE__,__func__,__LINE__)
#define malloc(sz) _zmalloc(sz,__FILE__,__func__,__LINE__)
#define realloc(ptr, new_size) _zrealloc(ptr, new_size,__FILE__,__func__,__LINE__)


#endif


#endif //_OPENCSTL_ZALLOC_H
