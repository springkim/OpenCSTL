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
#if !defined(_OPENCSTL_VAN_EMDE_BOAS_TREE_H)
#define _OPENCSTL_VAN_EMDE_BOAS_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

typedef uintptr_t u64;


#define VEB_EMPTY   (~(u64)0)
#define HM_INIT_CAP 16


typedef struct {
    u64 key;
    void *val;
    int used;
} HMEntry;

typedef struct {
    HMEntry *e;
    size_type64 cap, size;
} HashMap;

static u64 hm_hash(u64 k) {
    k = (k ^ (k >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    k = (k ^ (k >> 27)) * UINT64_C(0x94d049bb133111eb);
    return k ^ (k >> 31);
}

static HashMap *hm_new(void) {
    HashMap *h = (HashMap *) calloc(1, sizeof(HashMap));
    h->cap = HM_INIT_CAP;
    h->e = (HMEntry *) calloc(h->cap, sizeof(HMEntry));
    return h;
}

static void hm_free(HashMap *h) {
    free(h->e);
    free(h);
}


static void hm_set(HashMap *h, u64 key, void *val);

static void hm_grow(HashMap *h) {
    size_type64 oc = h->cap;
    HMEntry *oe = h->e;
    h->cap = oc * 2;
    h->e = (HMEntry *) calloc(h->cap, sizeof(HMEntry));
    h->size = 0;
    for (size_type64 i = 0; i < oc; i++)
        if (oe[i].used) hm_set(h, oe[i].key, oe[i].val);
    free(oe);
}

static void hm_set(HashMap *h, u64 k, void *v) {
    if (h->size * 2 >= h->cap) hm_grow(h);
    size_type64 i = (size_type64) (hm_hash(k) % h->cap);
    while (h->e[i].used && h->e[i].key != k)
        i = (i + 1) % h->cap;
    if (!h->e[i].used) h->size++;
    h->e[i] = (HMEntry){k, v, 1};
}

static void *hm_get(const HashMap *h, u64 k) {
    size_type64 i = (size_type64) (hm_hash(k) % h->cap);
    while (h->e[i].used) {
        if (h->e[i].key == k) return h->e[i].val;
        i = (i + 1) % h->cap;
    }
    return NULL;
}

static void hm_del(HashMap *h, u64 k) {
    size_type64 i = (size_type64) (hm_hash(k) % h->cap);
    while (h->e[i].used && h->e[i].key != k)
        i = (i + 1) % h->cap;
    if (!h->e[i].used) return;
    h->e[i].used = 0;
    h->size--;
    // 삭제 후 뒤따르는 항목들 재배치 (프로빙 체인 불변 유지)
    size_type64 j = (i + 1) % h->cap;
    while (h->e[j].used) {
        HMEntry t = h->e[j];
        h->e[j].used = 0;
        h->size--;
        hm_set(h, t.key, t.val);
        j = (j + 1) % h->cap;
    }
}


typedef struct VEB {
    int bits;
    u64 min;
    u64 max;
    struct VEB *sum;
    HashMap *cls;
} VEB;


static int lo_b(int b) { return b / 2; }
static int hi_b(int b) { return b - b / 2; }


static u64 vhi(u64 x, int b) { return x >> lo_b(b); }
static u64 vlo(u64 x, int b) { return x & (((u64) 1 << lo_b(b)) - 1); }
static u64 vidx(u64 h, u64 l, int b) { return (h << lo_b(b)) | l; }

static VEB *veb_new(int bits) {
    VEB *v = (VEB *) calloc(1, sizeof(VEB));
    v->bits = bits;
    v->min = VEB_EMPTY;
    v->max = VEB_EMPTY;
    if (bits > 1) v->cls = hm_new();
    return v;
}

// 전방 선언
static void veb_free(VEB *v);

static void cls_free_all(HashMap *hm) {
    for (size_type64 i = 0; i < hm->cap; i++)
        if (hm->e[i].used) veb_free((VEB *) hm->e[i].val);
    hm_free(hm);
}

static void veb_free(VEB *v) {
    if (!v) return;
    if (v->bits > 1) {
        veb_free(v->sum);
        if (v->cls) cls_free_all(v->cls);
    }
    free(v);
}

static int veb_empty(const VEB *v) { return v->min == VEB_EMPTY; }

static VEB *cls_get(VEB *v, u64 h) { return (VEB *) hm_get(v->cls, h); }


static VEB *cls_or_new(VEB *v, u64 h) {
    VEB *c = (VEB *) hm_get(v->cls, h);
    if (!c) {
        c = veb_new(lo_b(v->bits));
        hm_set(v->cls, h, c);
    }
    return c;
}


static VEB *sum_or_new(VEB *v) {
    if (!v->sum) v->sum = veb_new(hi_b(v->bits));
    return v->sum;
}


static void veb_ins(VEB *v, u64 x) {
    if (veb_empty(v)) {
        v->min = v->max = x;
        return;
    }

    // x가 현재 min보다 작으면 교환해 min 갱신
    // (min은 클러스터에 저장하지 않으므로 재귀 불필요)
    if (x < v->min) {
        u64 t = v->min;
        v->min = x;
        x = t;
    }

    // bits==1: universe {0,1}, 클러스터 없이 min/max로 처리
    if (v->bits == 1) {
        if (x > v->max) v->max = x;
        return;
    }

    u64 h = vhi(x, v->bits), l = vlo(x, v->bits);
    VEB *c = cls_or_new(v, h);
    // 클러스터가 비어있었으면 summary에 h 삽입
    if (veb_empty(c)) veb_ins(sum_or_new(v), h);
    veb_ins(c, l);
    if (x > v->max) v->max = x;
}


static void veb_del(VEB *v, u64 x) {
    // 원소가 1개뿐이면 바로 비움
    if (v->min == v->max) {
        v->min = v->max = VEB_EMPTY;
        return;
    }

    // bits==1: {0,1} 중 하나 제거
    if (v->bits == 1) {
        v->min = v->max = (x == 0) ? 1 : 0;
        return;
    }

    if (x == v->min) {
        // min 제거: 다음 최솟값을 클러스터에서 꺼내 min으로 승격
        if (!v->sum || veb_empty(v->sum)) {
            v->min = v->max = VEB_EMPTY;
            return;
        }
        u64 fh = v->sum->min; // 비어있지 않은 첫 클러스터
        VEB *fc = cls_get(v, fh);
        v->min = x = vidx(fh, fc->min, v->bits);
    }

    u64 h = vhi(x, v->bits), l = vlo(x, v->bits);
    VEB *c = cls_get(v, h);
    if (!c) return;
    veb_del(c, l);

    if (veb_empty(c)) {
        // 클러스터가 비면 해제하고 summary에서도 제거
        hm_del(v->cls, h);
        veb_free(c);
        if (v->sum) veb_del(v->sum, h);
        if (x == v->max) {
            if (!v->sum || veb_empty(v->sum)) v->max = v->min;
            else {
                u64 lh = v->sum->max; // 비어있지 않은 마지막 클러스터
                VEB *lc = cls_get(v, lh);
                v->max = vidx(lh, lc->max, v->bits);
            }
        }
    } else if (x == v->max) {
        v->max = vidx(h, c->max, v->bits);
    }
}


static u64 veb_pred(VEB *v, u64 x) {
    if (veb_empty(v) || x <= v->min) return VEB_EMPTY;
    if (x > v->max) return v->max;
    // bits==1: x==1, v->min==0 임이 보장됨
    if (v->bits == 1) return 0;

    u64 h = vhi(x, v->bits), l = vlo(x, v->bits);
    VEB *c = cls_get(v, h);

    // 같은 클러스터 내에 l보다 작은 원소가 있으면 그것을 반환
    if (c && !veb_empty(c) && l > c->min)
        return vidx(h, veb_pred(c, l), v->bits);

    // 이전 클러스터(summary 기준으로 h 미만)의 최댓값을 반환
    u64 ph = (v->sum && !veb_empty(v->sum)) ? veb_pred(v->sum, h) : VEB_EMPTY;
    if (ph == VEB_EMPTY) return (x > v->min) ? v->min : VEB_EMPTY;
    VEB *pc = cls_get(v, ph);
    return pc ? vidx(ph, pc->max, v->bits) : VEB_EMPTY;
}


static u64 veb_floor(VEB *v, u64 x) {
    if (veb_empty(v) || x < v->min) return VEB_EMPTY;
    if (x >= v->max) return v->max;
    if (x == v->min) return v->min;
    // pred(x+1) = x 이하의 최댓값
    // x < max 이므로 x+1 이 VEB_EMPTY 로 오버플로되지 않음
    return veb_pred(v, x + 1);
}


typedef enum CONTAINER_TYPE {
    CT_VECTOR,
    CT_LIST,
    CT_STACK,
    CT_QUEUE,
    CT_DEQUE,
    CT_SET,
    CT_MAP,
    CT_UNORDERED_SET,
    CT_UNORDERED_MAP,
    CT_JSON,
    CT_GLOB,
    CT_CSV,
    CT_BITSET,
} CONTAINER_TYPE;

typedef struct {
    void *a; // 구간 시작 (inclusive)
    void *b; // 구간 끝   (inclusive)
    CONTAINER_TYPE ctype;
    size_type64 type_size;
    char *type_name;
} Interval;

typedef struct {
    VEB *veb;
    HashMap *data;
} IntervalVEB;

typedef struct {
    void *p1, *p2;
    char *tombstone;
    int type_size;
    bool used;
} HashtableManager;

typedef struct {
    VEB *veb;
    HashMap *data;
} HTMVEB;

typedef struct {
    unsigned char *p1;
    unsigned char *p2;
    int type_size;
} GInterval;

typedef struct {
    VEB *veb;
    HashMap *data;
} GIntervalVEB;

HTMVEB *htm_new(void) {
    HTMVEB *iv = (HTMVEB *) calloc(1, sizeof(HTMVEB));
    iv->veb = veb_new(64);
    iv->data = hm_new();
    return iv;
}

void htm_free(HTMVEB *iv) {
    for (size_type64 i = 0; i < iv->data->cap; i++)
        if (iv->data->e[i].used)
            free(iv->data->e[i].val);
    hm_free(iv->data);
    veb_free(iv->veb);
    free(iv);
}

void htm_insert(HTMVEB *iv, void *a, void *b, char *tombstone, int type_size) {
    u64 k = (u64) (uintptr_t) a;
    HashtableManager *it = (HashtableManager *) hm_get(iv->data, k);
    if (!it) {
        it = (HashtableManager *) malloc(sizeof(HashtableManager));
        hm_set(iv->data, k, it);
        veb_ins(iv->veb, k);
    }
    it->p1 = a;
    it->p2 = b;
    it->tombstone = tombstone;
    it->used = true;
    it->type_size = type_size;
}


void htm_erase(HTMVEB *iv, void *a) {
    u64 k = (u64) (uintptr_t) a;
    HashtableManager *it = (HashtableManager *) hm_get(iv->data, k);
    if (!it) return;
    free(it);
    hm_del(iv->data, k);
    veb_del(iv->veb, k);
}


HashtableManager *htm_find(HTMVEB *iv, void *x) {
    u64 key = (u64) (uintptr_t) x;
    u64 start = veb_floor(iv->veb, key);
    if (start == VEB_EMPTY) return NULL;
    HashtableManager *it = (HashtableManager *) hm_get(iv->data, start);
    if (!it) return NULL;
    return ((uintptr_t) x <= (uintptr_t) it->p2) ? it : NULL;
}

// ██╗███╗░░██╗████████╗███████╗██████╗░██╗░░░██╗░█████╗░██╗░░░░░
// ██║████╗░██║╚══██╔══╝██╔════╝██╔══██╗██║░░░██║██╔══██╗██║░░░░░
// ██║██╔██╗██║░░░██║░░░█████╗░░██████╔╝╚██╗░██╔╝███████║██║░░░░░
// ██║██║╚████║░░░██║░░░██╔══╝░░██╔══██╗░╚████╔╝░██╔══██║██║░░░░░
// ██║██║░╚███║░░░██║░░░███████╗██║░░██║░░╚██╔╝░░██║░░██║███████╗
// ╚═╝╚═╝░░╚══╝░░░╚═╝░░░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚══════╝


IntervalVEB *iveb_new(void) {
    IntervalVEB *iv = (IntervalVEB *) calloc(1, sizeof(IntervalVEB));
    iv->veb = veb_new(64);
    iv->data = hm_new();
    return iv;
}


void iveb_free(IntervalVEB *iv) {
    for (size_type64 i = 0; i < iv->data->cap; i++)
        if (iv->data->e[i].used)
            free(iv->data->e[i].val);
    hm_free(iv->data);
    veb_free(iv->veb);
    free(iv);
}


void iveb_insert(IntervalVEB *iv, void *a, void *b, CONTAINER_TYPE ctype, size_type64 type_size, char *type_name) {
    u64 k = (u64) (uintptr_t) a;
    Interval *it = (Interval *) hm_get(iv->data, k);
    if (!it) {
        it = (Interval *) malloc(sizeof(Interval));
        hm_set(iv->data, k, it);
        veb_ins(iv->veb, k);
    }
    it->a = a;
    it->b = b;
    it->ctype = ctype;
    it->type_size = type_size;
    it->type_name = type_name;
}


void iveb_erase(IntervalVEB *iv, void *a) {
    u64 k = (u64) (uintptr_t) a;
    Interval *it = (Interval *) hm_get(iv->data, k);
    if (!it) return;
    free(it);
    hm_del(iv->data, k);
    veb_del(iv->veb, k);
}


Interval *iveb_find(IntervalVEB *iv, void *x) {
    if (iv == NULL) return NULL;
    u64 key = (u64) (uintptr_t) x;
    u64 start = veb_floor(iv->veb, key);
    if (start == VEB_EMPTY) return NULL;
    Interval *it = (Interval *) hm_get(iv->data, start);
    if (!it) return NULL;
    return ((uintptr_t) x <= (uintptr_t) it->b) ? it : NULL;
}

//==================================

#include "galloc.h"

// Power-of-two bucket allocator over OCSTL_DBLOCK.
// Each alloc is rounded up to the next power of two; bucket indices are
// the log2 of the rounded size minus _DBLOCK_MIN_SHIFT. Bump pointer
// for fresh slots, LIFO free list per bucket for reuse.
#define _DBLOCK_MIN_SHIFT    4   // 16 B
#define _DBLOCK_MAX_SHIFT    20  // 1 MB
#define _DBLOCK_NUM_BUCKETS  (_DBLOCK_MAX_SHIFT - _DBLOCK_MIN_SHIFT + 1)

typedef struct _OCSTLDBlockNode {
    struct _OCSTLDBlockNode *next;
} _OCSTLDBlockNode;

static size_type64 _ocstl_dblock_bump = 0;
static _OCSTLDBlockNode *_ocstl_dblock_freelist[_DBLOCK_NUM_BUCKETS] = {NULL};

static int _ocstl_dblock_bucket(size_type64 sz) {
    int sh = _DBLOCK_MIN_SHIFT;
    size_type64 s = (size_type64) 1 << sh;
    while (s < sz) {
        s <<= 1;
        sh++;
    }
    return sh - _DBLOCK_MIN_SHIFT;
}

static void *_ocstl_dblock_alloc(size_type64 sz) {
    int b = _ocstl_dblock_bucket(sz);
    size_type64 real = (size_type64) 1 << (b + _DBLOCK_MIN_SHIFT);
    if (_ocstl_dblock_freelist[b]) {
        _OCSTLDBlockNode *n = _ocstl_dblock_freelist[b];
        _ocstl_dblock_freelist[b] = n->next;
        memset(n, 0, real);
        return n;
    }
    if (_ocstl_dblock_bump + real > ___OCSTL_1MB) return NULL;
    void *p = &OCSTL_DBLOCK[_ocstl_dblock_bump];
    _ocstl_dblock_bump += real;
    memset(p, 0, real);
    return p;
}

static void _ocstl_dblock_free(void *p, size_type64 sz) {
    if (!p) return;
    int b = _ocstl_dblock_bucket(sz);
    _OCSTLDBlockNode *n = (_OCSTLDBlockNode *) p;
    n->next = _ocstl_dblock_freelist[b];
    _ocstl_dblock_freelist[b] = n;
}

// ---- DBLOCK-backed HashMap (giveb-only; reuses pure helpers hm_hash, hm_get) ----

static void giveb_hm_set(HashMap *h, u64 key, void *val); // fwd

static HashMap *giveb_hm_new(void) {
    HashMap *h = (HashMap *) _ocstl_dblock_alloc(sizeof(HashMap));
    h->cap = HM_INIT_CAP;
    h->e = (HMEntry *) _ocstl_dblock_alloc(h->cap * sizeof(HMEntry));
    return h;
}

static void giveb_hm_free(HashMap *h) {
    _ocstl_dblock_free(h->e, h->cap * sizeof(HMEntry));
    _ocstl_dblock_free(h, sizeof(HashMap));
}

static void giveb_hm_grow(HashMap *h) {
    size_type64 oc = h->cap;
    HMEntry *oe = h->e;
    h->cap = oc * 2;
    h->e = (HMEntry *) _ocstl_dblock_alloc(h->cap * sizeof(HMEntry));
    h->size = 0;
    for (size_type64 i = 0; i < oc; i++)
        if (oe[i].used) giveb_hm_set(h, oe[i].key, oe[i].val);
    _ocstl_dblock_free(oe, oc * sizeof(HMEntry));
}

static void giveb_hm_set(HashMap *h, u64 k, void *v) {
    if (h->size * 2 >= h->cap) giveb_hm_grow(h);
    size_type64 i = (size_type64) (hm_hash(k) % h->cap);
    while (h->e[i].used && h->e[i].key != k)
        i = (i + 1) % h->cap;
    if (!h->e[i].used) h->size++;
    h->e[i] = (HMEntry){k, v, 1};
}

static void giveb_hm_del(HashMap *h, u64 k) {
    size_type64 i = (size_type64) (hm_hash(k) % h->cap);
    while (h->e[i].used && h->e[i].key != k)
        i = (i + 1) % h->cap;
    if (!h->e[i].used) return;
    h->e[i].used = 0;
    h->size--;
    size_type64 j = (i + 1) % h->cap;
    while (h->e[j].used) {
        HMEntry t = h->e[j];
        h->e[j].used = 0;
        h->size--;
        giveb_hm_set(h, t.key, t.val);
        j = (j + 1) % h->cap;
    }
}

// ---- DBLOCK-backed VEB (giveb-only; reuses pure helpers veb_empty, cls_get,
// veb_pred, veb_floor, veb_succ, vhi, vlo, vidx, lo_b, hi_b) ----

static VEB *giveb_veb_new(int bits) {
    VEB *v = (VEB *) _ocstl_dblock_alloc(sizeof(VEB));
    v->bits = bits;
    v->min = VEB_EMPTY;
    v->max = VEB_EMPTY;
    if (bits > 1) v->cls = giveb_hm_new();
    return v;
}

static void giveb_veb_free(VEB *v);

static void giveb_cls_free_all(HashMap *hm) {
    for (size_type64 i = 0; i < hm->cap; i++)
        if (hm->e[i].used) giveb_veb_free((VEB *) hm->e[i].val);
    giveb_hm_free(hm);
}

static void giveb_veb_free(VEB *v) {
    if (!v) return;
    if (v->bits > 1) {
        giveb_veb_free(v->sum);
        if (v->cls) giveb_cls_free_all(v->cls);
    }
    _ocstl_dblock_free(v, sizeof(VEB));
}

static VEB *giveb_cls_or_new(VEB *v, u64 h) {
    VEB *c = (VEB *) hm_get(v->cls, h);
    if (!c) {
        c = giveb_veb_new(lo_b(v->bits));
        giveb_hm_set(v->cls, h, c);
    }
    return c;
}

static VEB *giveb_sum_or_new(VEB *v) {
    if (!v->sum) v->sum = giveb_veb_new(hi_b(v->bits));
    return v->sum;
}

static void giveb_veb_ins(VEB *v, u64 x) {
    if (veb_empty(v)) {
        v->min = v->max = x;
        return;
    }
    if (x < v->min) {
        u64 t = v->min;
        v->min = x;
        x = t;
    }
    if (v->bits == 1) {
        if (x > v->max) v->max = x;
        return;
    }
    u64 h = vhi(x, v->bits), l = vlo(x, v->bits);
    VEB *c = giveb_cls_or_new(v, h);
    if (veb_empty(c)) giveb_veb_ins(giveb_sum_or_new(v), h);
    giveb_veb_ins(c, l);
    if (x > v->max) v->max = x;
}

static void giveb_veb_del(VEB *v, u64 x) {
    if (v->min == v->max) {
        v->min = v->max = VEB_EMPTY;
        return;
    }
    if (v->bits == 1) {
        v->min = v->max = (x == 0) ? 1 : 0;
        return;
    }
    if (x == v->min) {
        if (!v->sum || veb_empty(v->sum)) {
            v->min = v->max = VEB_EMPTY;
            return;
        }
        u64 fh = v->sum->min;
        VEB *fc = cls_get(v, fh);
        v->min = x = vidx(fh, fc->min, v->bits);
    }
    u64 h = vhi(x, v->bits), l = vlo(x, v->bits);
    VEB *c = cls_get(v, h);
    if (!c) return;
    giveb_veb_del(c, l);
    if (veb_empty(c)) {
        giveb_hm_del(v->cls, h);
        giveb_veb_free(c);
        if (v->sum) giveb_veb_del(v->sum, h);
        if (x == v->max) {
            if (!v->sum || veb_empty(v->sum)) v->max = v->min;
            else {
                u64 lh = v->sum->max;
                VEB *lc = cls_get(v, lh);
                v->max = vidx(lh, lc->max, v->bits);
            }
        }
    } else if (x == v->max) {
        v->max = vidx(h, c->max, v->bits);
    }
}

// ---- giveb public API (DBLOCK-only) ----

GIntervalVEB *giveb_new(void) {
    GIntervalVEB *iv = (GIntervalVEB *) _ocstl_dblock_alloc(sizeof(GIntervalVEB));
    iv->veb = giveb_veb_new(64);
    iv->data = giveb_hm_new();
    return iv;
}


void giveb_free(GIntervalVEB *iv) {
    for (size_type64 i = 0; i < iv->data->cap; i++)
        if (iv->data->e[i].used)
            _ocstl_dblock_free(iv->data->e[i].val, sizeof(GInterval));
    giveb_hm_free(iv->data);
    giveb_veb_free(iv->veb);
    _ocstl_dblock_free(iv, sizeof(GIntervalVEB));
}


void giveb_insert(GIntervalVEB *iv, void *a, void *b, int type_size) {
    u64 k = (u64) (uintptr_t) a;
    GInterval *it = (GInterval *) hm_get(iv->data, k);
    if (!it) {
        it = (GInterval *) _ocstl_dblock_alloc(sizeof(GInterval));
        giveb_hm_set(iv->data, k, it);
        giveb_veb_ins(iv->veb, k);
    }
    it->p1 = a;
    it->p2 = b;
    it->type_size = type_size;
}


void giveb_erase(GIntervalVEB *iv, void *a) {
    u64 key = (u64) (uintptr_t) a;
    u64 start = veb_floor(iv->veb, key);
    if (start == VEB_EMPTY) return;
    GInterval *it = (GInterval *) hm_get(iv->data, start);
    if (!it) return;
    if ((uintptr_t) a > (uintptr_t) it->p2) return;
    _ocstl_dblock_free(it, sizeof(GInterval));
    giveb_hm_del(iv->data, start);
    giveb_veb_del(iv->veb, start);
}

GInterval *giveb_find(GIntervalVEB *iv, void *x) {
    if (iv==NULL) return NULL;
    u64 key = (u64) (uintptr_t) x;
    u64 start = veb_floor(iv->veb, key);
    if (start == VEB_EMPTY) return NULL;
    GInterval *it = (GInterval *) hm_get(iv->data, start);
    if (!it) return NULL;
    return ((uintptr_t) x <= (uintptr_t) it->p2) ? it : NULL;
}

static u64 veb_succ(VEB *v, u64 x) {
    if (veb_empty(v) || x >= v->max) return VEB_EMPTY;
    if (x < v->min) return v->min;
    if (v->bits == 1) return 1; // x==0, max==1 보장

    u64 h = vhi(x, v->bits), l = vlo(x, v->bits);
    VEB *c = cls_get(v, h);

    // 같은 클러스터 내에 l보다 큰 원소가 있으면 반환
    if (c && !veb_empty(c) && l < c->max) {
        u64 nl = veb_succ(c, l);
        return vidx(h, nl, v->bits);
    }

    // 다음 비어있지 않은 클러스터의 최솟값 반환
    u64 nh = (v->sum && !veb_empty(v->sum))
                 ? veb_succ(v->sum, h)
                 : VEB_EMPTY;
    if (nh == VEB_EMPTY) return VEB_EMPTY;
    VEB *nc = cls_get(v, nh);
    if (!nc) return VEB_EMPTY;
    return vidx(nh, nc->min, v->bits);
}

void *giveb_alloc(GIntervalVEB *iv, size_type64 sz, size_type64 type_size) {
    if (!iv || sz == 0) return NULL;

    void *block_start = (void *) OCSTL_GBLOCK;
    void *block_end = (void *) (OCSTL_GBLOCK + ___OCSTL_512MB);

    // GBLOCK 전체가 비어있으면 시작 주소 바로 반환
    if (veb_empty(iv->veb)) {
        if (sz <= ___OCSTL_512MB) {
            giveb_insert(iv, block_start,
                         (void *) ((unsigned char *) block_start + sz - 1), type_size);
            return block_start;
        }
        return NULL;
    }

    u64 cur_key = iv->veb->min;

    // GBLOCK 시작 ~ 첫 번째 구간 사이 확인
    GInterval *first = (GInterval *) hm_get(iv->data, cur_key);
    if (first) {
        u64 gap_start = (u64) (uintptr_t) block_start;
        u64 gap_end = (u64) (uintptr_t) first->p1;
        if (gap_end > gap_start && gap_end - gap_start >= sz) {
            void *p = (void *) (uintptr_t) gap_start;
            giveb_insert(iv, p, (void *) ((unsigned char *) p + sz - 1), type_size);
            return p;
        }
    }

    // 등록된 구간 사이 빈 틈 탐색
    while (cur_key != VEB_EMPTY) {
        GInterval *cur = (GInterval *) hm_get(iv->data, cur_key);
        if (!cur) break;

        u64 gap_start = (u64) (uintptr_t) cur->p2 + 1;
        u64 next_key = veb_succ(iv->veb, cur_key);

        u64 gap_end = (next_key != VEB_EMPTY)
                          ? (u64) (uintptr_t) ((GInterval *) hm_get(iv->data, next_key))->p1
                          : (u64) (uintptr_t) block_end;

        // GBLOCK 범위 clamp
        if (gap_start < (u64) (uintptr_t) block_start)
            gap_start = (u64) (uintptr_t) block_start;
        if (gap_end > (u64) (uintptr_t) block_end)
            gap_end = (u64) (uintptr_t) block_end;

        if (gap_end > gap_start && gap_end - gap_start >= sz) {
            void *p = (void *) (uintptr_t) gap_start;
            giveb_insert(iv, p, (void *) ((unsigned char *) p + sz - 1), type_size);
            return p;
        }

        if (next_key == VEB_EMPTY) break;
        cur_key = next_key;
    }

    return NULL; // GBLOCK 내 sz 크기의 빈 공간 없음
}


static GIntervalVEB *giveb = NULL;


static IntervalVEB *iveb = NULL;

static HTMVEB *htm = NULL;

void __opencstl_giveb_destroy(void) {
    giveb_free(giveb);
}

void __cstl_htm_destroy(void) {
    htm_free(htm);
}

void __opencstl_iveb_destroy(void) {
    iveb_free(iveb);
}
#ifdef OPENCSTL_TRACER
void *_garray_ctx = NULL;
#define galloc(SZ,tsz)  (_garray_ctx=_galloc(SZ,tsz));zinsert(_garray_ctx,__FILE__,__func__,__LINE__);
#define gfree(P) _gfree(P);zremove(P);
#else
#define galloc(SZ,TSZ) _galloc(SZ ,TSZ)
#define gfree(P) _gfree(P)
#endif
static void *_galloc(size_type64 sz, size_type64 type_size) {
    bool giveb_init = false;
    if (giveb == NULL) {
        giveb = giveb_new();
        giveb_init = true;
    }
    void *p = giveb_alloc(giveb, sz, type_size);
    if (p == NULL) {
        return NULL;
    }
    giveb_insert(giveb, p, (char *) p + (sz), type_size);
    if (giveb_init) {
        atexit(__opencstl_giveb_destroy);
    }

    return p;
}

static void _gfree(void *p) {
    giveb_erase(giveb, p);
}
#endif //_OPENCSTL_VAN_EMDE_BOAS_TREE_H
