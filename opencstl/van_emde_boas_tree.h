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
    CT_GLOB
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
    u64 key = (u64) (uintptr_t) x;
    u64 start = veb_floor(iv->veb, key);
    if (start == VEB_EMPTY) return NULL;
    Interval *it = (Interval *) hm_get(iv->data, start);
    if (!it) return NULL;
    return ((uintptr_t) x <= (uintptr_t) it->b) ? it : NULL;
}

static IntervalVEB *iveb = NULL;
static HTMVEB *htm = NULL;

void __opencstl_iveb_destroy(void) {
    iveb_free(iveb);
}
#endif //_OPENCSTL_VAN_EMDE_BOAS_TREE_H
