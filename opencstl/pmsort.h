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
#if !defined(_OPENCSTL_PMSORT_H)
#define _OPENCSTL_PMSORT_H
#include "pthread_cc.h"
#include <stdlib.h>
#include <string.h>
#include "msort.h"
#ifndef PS_MAX_DEPTH
#define PS_MAX_DEPTH 4
#endif

#ifndef PS_SEQ_CUTOFF
#define PS_SEQ_CUTOFF 4096
#endif

typedef int (*ps_cmp_t)(const void *, const void *);

struct ps_args {
    char *base;
    char *buf;
    size_t n;
    size_t sz;
    ps_cmp_t cmp;
    int depth;
};

static void ps_merge(char *base, char *buf, size_t n1, size_t n2,
                     size_t sz, ps_cmp_t cmp) {
    char *l = base;
    char *r = base + n1 * sz;
    char *l_end = r;
    char *r_end = r + n2 * sz;
    char *d = buf;
    while (l < l_end && r < r_end) {
        if (cmp(r, l) < 0) {
            memcpy(d, r, sz);
            r += sz;
        } else {
            memcpy(d, l, sz);
            l += sz;
        }
        d += sz;
    }
    if (l < l_end)
        memcpy(d, l, (size_t) (l_end - l));
    if (r < r_end)
        memcpy(d, r, (size_t) (r_end - r));
    memcpy(base, buf, (n1 + n2) * sz);
}

static void *ps_run(void *p) {
    struct ps_args *a = (struct ps_args *) p;

    if (a->n < 2) return NULL;
    if (a->n <= PS_SEQ_CUTOFF || a->depth >= PS_MAX_DEPTH) {
        qsort(a->base, a->n, a->sz, a->cmp);
        return NULL;
    }

    size_t mid = a->n / 2;
    struct ps_args left = {
        a->base, a->buf, mid, a->sz, a->cmp, a->depth + 1
    };
    struct ps_args right = {
        a->base + mid * a->sz, a->buf + mid * a->sz,
        a->n - mid, a->sz, a->cmp, a->depth + 1
    };

    pthread_t tid;
    int spawned = (pthread_create(&tid, NULL, ps_run, &left) == 0);
    if (!spawned) ps_run(&left);
    ps_run(&right);
    if (spawned) pthread_join(tid, NULL);

    ps_merge(a->base, a->buf, mid, a->n - mid, a->sz, a->cmp);
    return NULL;
}

void pmsort(void *mem, const size_t len, const size_t size_elem,
            int (*cmp)(const void *, const void *)) {
    if (len < 2) return;
    if (len <= PS_SEQ_CUTOFF) {
        msort(mem, len, size_elem, cmp);
        return;
    }
    char *buf = (char *) malloc(len * size_elem);
    if (!buf) {
        msort(mem, len, size_elem, cmp);
        return;
    }
    struct ps_args root = {
        (char *) mem, buf, len, size_elem, cmp, 0
    };
    ps_run(&root);
    free(buf);
}

#undef PS_MAX_DEPTH
#undef PS_SEQ_CUTOFF
#endif
