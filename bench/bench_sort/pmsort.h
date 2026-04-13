//
// Created by spring on 2026. 4. 12..
//

#ifndef BENCH_SORT_PMSORT_H
#define BENCH_SORT_PMSORT_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
// clock_gettime 대신 gettimeofday 사용 (C99 + 모든 POSIX 플랫폼)
#include <sys/time.h>
 
#define MAX_THREADS 2
 
typedef int (*CmpFn)(const void *, const void *);
 
// ---------------------------------------------------------------
// 커스텀 Barrier
// ---------------------------------------------------------------
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int             count;
    int             total;
    int             generation;
} Barrier;
 
static int barrier_init(Barrier *b, int total) {
    int rc;
    rc = pthread_mutex_init(&b->mutex, NULL);
    if (rc != 0) return rc;
    rc = pthread_cond_init(&b->cond, NULL);
    if (rc != 0) { pthread_mutex_destroy(&b->mutex); return rc; }
    b->count      = 0;
    b->total      = total;
    b->generation = 0;
    return 0;
}
 
static void barrier_wait(Barrier *b) {
    pthread_mutex_lock(&b->mutex);
    int gen = b->generation;
    b->count++;
    if (b->count == b->total) {
        b->count = 0;
        b->generation++;
        pthread_cond_broadcast(&b->cond);
    } else {
        while (gen == b->generation)
            pthread_cond_wait(&b->cond, &b->mutex);
    }
    pthread_mutex_unlock(&b->mutex);
}
 
static void barrier_destroy(Barrier *b) {
    pthread_cond_destroy(&b->cond);
    pthread_mutex_destroy(&b->mutex);
}
 
// ---------------------------------------------------------------
// Thread Pool 공유 상태
// ---------------------------------------------------------------
typedef struct {
    char   **src_ptr;
    char   **dst_ptr;
    size_t   nel;
    size_t   width;
    CmpFn    compar;
    size_t   seg_nel;
    size_t   num_tasks;
    int      num_threads;
    Barrier  bar_start;
    Barrier  bar_done;
    int      done;
} PoolState;
 
typedef struct {
    PoolState *pool;
    int        tid;
} ThreadArg;
 
// ---------------------------------------------------------------
// 단일 merge
// ---------------------------------------------------------------
static void merge_one(
    const char *src, char *dst,
    size_t li, size_t mi, size_t ri,
    size_t width, CmpFn compar)
{
    size_t i = li, j = mi, k = li;
    while (i < mi && j < ri) {
        const void *a = src + i * width;
        const void *b = src + j * width;
        if (compar(a, b) <= 0)
            { memcpy(dst + k * width, a, width); k++; i++; }
        else
            { memcpy(dst + k * width, b, width); k++; j++; }
    }
    while (i < mi) { memcpy(dst + k * width, src + i * width, width); k++; i++; }
    while (j < ri) { memcpy(dst + k * width, src + j * width, width); k++; j++; }
}
 
// ---------------------------------------------------------------
// 워커 스레드
// ---------------------------------------------------------------
static void *worker(void *arg) {
    ThreadArg *ta   = (ThreadArg *)arg;
    PoolState *pool = ta->pool;
    int        tid  = ta->tid;
 
    while (1) {
        barrier_wait(&pool->bar_start);
        if (pool->done) break;
 
        const char *src     = *pool->src_ptr;
        char       *dst     = *pool->dst_ptr;
        size_t      nel     = pool->nel;
        size_t      width   = pool->width;
        CmpFn       compar  = pool->compar;
        size_t      seg_nel = pool->seg_nel;
        size_t      total   = pool->num_tasks;
        size_t      nthrd   = (size_t)pool->num_threads;
 
        size_t base      = total / nthrd;
        size_t remainder = total % nthrd;
        size_t utid      = (size_t)tid;
        size_t t_start   = utid * base + (utid < remainder ? utid : remainder);
        size_t t_count   = base + (utid < remainder ? 1u : 0u);
        size_t t;
 
        for (t = t_start; t < t_start + t_count; t++) {
            size_t li = t * seg_nel;
            size_t ri = li + seg_nel < nel ? li + seg_nel : nel;
            size_t mi = li + seg_nel / 2;
 
            if (mi >= nel)
                memcpy(dst + li * width, src + li * width, (ri - li) * width);
            else
                merge_one(src, dst, li, mi, ri, width, compar);
        }
 
        barrier_wait(&pool->bar_done);
    }
    return NULL;
}
 
// ---------------------------------------------------------------
// msort
// ---------------------------------------------------------------
void pmsort(void *base, size_t nel, size_t width,
           int (*compar)(const void *, const void *))
{
    int num_threads, i, rc;
    size_t half;
    char *buf, *src, *dst, *tmp;
    pthread_t tids[MAX_THREADS];
    ThreadArg targs[MAX_THREADS];
    PoolState pool;
 
    if (nel <= 1) return;
 
    num_threads = 8;
    if (num_threads > MAX_THREADS) num_threads = MAX_THREADS;
 
    buf = (char *)malloc(nel * width);
    if (!buf) { perror("msort: malloc"); return; }
 
    src = (char *)base;
    dst = buf;
 
    pool.src_ptr     = &src;
    pool.dst_ptr     = &dst;
    pool.nel         = nel;
    pool.width       = width;
    pool.compar      = compar;
    pool.num_threads = num_threads;
    pool.done        = 0;
 
    rc = barrier_init(&pool.bar_start, num_threads + 1);
    if (rc != 0) { fprintf(stderr, "msort: bar_start init failed: %d\n", rc); free(buf); return; }
    rc = barrier_init(&pool.bar_done, num_threads + 1);
    if (rc != 0) { fprintf(stderr, "msort: bar_done init failed: %d\n", rc); barrier_destroy(&pool.bar_start); free(buf); return; }
 
    for (i = 0; i < num_threads; i++) {
        targs[i].pool = &pool;
        targs[i].tid  = i;
        rc = pthread_create(&tids[i], NULL, worker, &targs[i]);
        if (rc != 0) {
            // 생성 실패 시 지금까지 만든 스레드 수로 줄여서 계속
            fprintf(stderr, "msort: pthread_create[%d] failed: %d, using %d threads\n", i, rc, i);
            num_threads = i;
            pool.num_threads = i;
            // barrier total 재설정 불가 → 단일 스레드로 폴백
            if (num_threads == 0) {
                barrier_destroy(&pool.bar_start);
                barrier_destroy(&pool.bar_done);
                free(buf);
                return;
            }
            break;
        }
    }
 
    for (half = 1; half < nel; half <<= 1) {
        size_t seg_nel   = half << 1;
        size_t num_tasks = (nel + seg_nel - 1) / seg_nel;
 
        pool.seg_nel   = seg_nel;
        pool.num_tasks = num_tasks;
 
        barrier_wait(&pool.bar_start);
        barrier_wait(&pool.bar_done);
 
        tmp = src; src = dst; dst = tmp;
    }
 
    pool.done = 1;
    barrier_wait(&pool.bar_start);
 
    for (i = 0; i < num_threads; i++) pthread_join(tids[i], NULL);
 
    barrier_destroy(&pool.bar_start);
    barrier_destroy(&pool.bar_done);
 
    if (src != (char *)base)
        memcpy(base, src, nel * width);
 
    free(buf);
}

#endif //BENCH_SORT_PMSORT_H