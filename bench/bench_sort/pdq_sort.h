#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef int (*pdq_cmp_fn)(const void *, const void *);

#define PDQ_INSERTION_SORT_THRESHOLD   24
#define PDQ_NINTHER_THRESHOLD         128
#define PDQ_PARTIAL_INSERTION_LIMIT     8

typedef enum {
    PDQ_SWAP_BYTES = 0,
    PDQ_SWAP_4     = 4,
    PDQ_SWAP_8     = 8,
    PDQ_SWAP_16    = 16
} pdq_swap_mode_t;

typedef struct {
    size_t size;
    pdq_cmp_fn cmp;
    pdq_swap_mode_t swap_mode;
    unsigned char *tmp;
    unsigned char *pivot;
} pdq_ctx;

/* ------------------------------------------------------------------ */
/* low-level helpers                                                   */
/* ------------------------------------------------------------------ */

static inline unsigned char *pdq_elem(unsigned char *base, size_t i, size_t size) {
    return base + i * size;
}

static inline int pdq_aligned(const void *p, size_t a) {
    return (((uintptr_t)p) & (a - 1)) == 0;
}

static inline pdq_swap_mode_t pdq_choose_swap_mode(void *base, size_t size) {
    if (size == 4  && pdq_aligned(base, 4)) return PDQ_SWAP_4;
    if (size == 8  && pdq_aligned(base, 8)) return PDQ_SWAP_8;
    if (size == 16 && pdq_aligned(base, 8)) return PDQ_SWAP_16;
    return PDQ_SWAP_BYTES;
}

static inline void pdq_swap_bytes(unsigned char *a, unsigned char *b, size_t size) {
    while (size--) { unsigned char t = *a; *a++ = *b; *b++ = t; }
}

static inline void pdq_swap4(unsigned char *a, unsigned char *b) {
    uint32_t t = *(uint32_t *)a;
    *(uint32_t *)a = *(uint32_t *)b;
    *(uint32_t *)b = t;
}

static inline void pdq_swap8(unsigned char *a, unsigned char *b) {
    uint64_t t = *(uint64_t *)a;
    *(uint64_t *)a = *(uint64_t *)b;
    *(uint64_t *)b = t;
}

static inline void pdq_swap16(unsigned char *a, unsigned char *b) {
    uint64_t a0 = *(uint64_t *)(a + 0), a1 = *(uint64_t *)(a + 8);
    *(uint64_t *)(a + 0) = *(uint64_t *)(b + 0);
    *(uint64_t *)(a + 8) = *(uint64_t *)(b + 8);
    *(uint64_t *)(b + 0) = a0;
    *(uint64_t *)(b + 8) = a1;
}

static inline void pdq_swap(pdq_ctx *ctx, unsigned char *a, unsigned char *b) {
    switch (ctx->swap_mode) {
        case PDQ_SWAP_4:  pdq_swap4(a, b);  break;
        case PDQ_SWAP_8:  pdq_swap8(a, b);  break;
        case PDQ_SWAP_16: pdq_swap16(a, b); break;
        default:          pdq_swap_bytes(a, b, ctx->size); break;
    }
}

static inline size_t pdq_floor_log2(size_t n) {
    size_t r = 0;
    while (n > 1) { n >>= 1; ++r; }
    return r;
}

/* ------------------------------------------------------------------ */
/* insertion sort                                                      */
/* ------------------------------------------------------------------ */

/* guarded binary insertion sort (leftmost partitions) */
static void pdq_insertion_sort(pdq_ctx *ctx, unsigned char *base, size_t n) {
    if (n < 2) return;
    const size_t size = ctx->size;
    unsigned char *tmp = ctx->tmp;

    for (size_t i = 1; i < n; ++i) {
        unsigned char *cur = pdq_elem(base, i, size);
        if (ctx->cmp(pdq_elem(base, i - 1, size), cur) <= 0) continue;

        memcpy(tmp, cur, size);

        /* binary search for insertion point */
        size_t lo = 0, hi = i;
        while (lo < hi) {
            size_t mid = lo + ((hi - lo) >> 1);
            if (ctx->cmp(tmp, pdq_elem(base, mid, size)) < 0) hi = mid;
            else lo = mid + 1;
        }

        memmove(pdq_elem(base, lo + 1, size), pdq_elem(base, lo, size),
                (i - lo) * size);
        memcpy(pdq_elem(base, lo, size), tmp, size);
    }
}

/* unguarded binary insertion sort (non-leftmost partitions) */
static void pdq_unguarded_insertion_sort(pdq_ctx *ctx, unsigned char *base, size_t n) {
    if (n < 2) return;
    const size_t size = ctx->size;
    unsigned char *tmp = ctx->tmp;

    for (size_t i = 1; i < n; ++i) {
        unsigned char *cur = pdq_elem(base, i, size);
        if (ctx->cmp(pdq_elem(base, i - 1, size), cur) <= 0) continue;

        memcpy(tmp, cur, size);

        size_t lo = 0, hi = i;
        while (lo < hi) {
            size_t mid = lo + ((hi - lo) >> 1);
            if (ctx->cmp(tmp, pdq_elem(base, mid, size)) < 0) hi = mid;
            else lo = mid + 1;
        }

        memmove(pdq_elem(base, lo + 1, size), pdq_elem(base, lo, size),
                (i - lo) * size);
        memcpy(pdq_elem(base, lo, size), tmp, size);
    }
}

static int pdq_partial_insertion_sort(pdq_ctx *ctx, unsigned char *base, size_t n) {
    if (n < 2) return 1;
    const size_t size = ctx->size;
    unsigned char *tmp = ctx->tmp;
    size_t moves = 0;

    for (size_t i = 1; i < n; ++i) {
        unsigned char *cur = pdq_elem(base, i, size);
        if (ctx->cmp(pdq_elem(base, i - 1, size), cur) <= 0) continue;

        memcpy(tmp, cur, size);
        size_t j = i;
        do {
            memmove(pdq_elem(base, j, size), pdq_elem(base, j - 1, size), size);
            --j;
            if (++moves > PDQ_PARTIAL_INSERTION_LIMIT) return 0;
        } while (j > 0 && ctx->cmp(pdq_elem(base, j - 1, size), tmp) > 0);
        memcpy(pdq_elem(base, j, size), tmp, size);
    }
    return 1;
}

/* ------------------------------------------------------------------ */
/* pivot selection                                                     */
/* ------------------------------------------------------------------ */

static int pdq_sort2(pdq_ctx *ctx, unsigned char *base, size_t i, size_t j) {
    unsigned char *a = pdq_elem(base, i, ctx->size);
    unsigned char *b = pdq_elem(base, j, ctx->size);
    if (ctx->cmp(b, a) < 0) { pdq_swap(ctx, a, b); return 1; }
    return 0;
}

static int pdq_sort3(pdq_ctx *ctx, unsigned char *base, size_t a, size_t b, size_t c) {
    int s = 0;
    s += pdq_sort2(ctx, base, a, b);
    s += pdq_sort2(ctx, base, b, c);
    s += pdq_sort2(ctx, base, a, b);
    return s;
}

/*
 * Selects pivot, moves it to base[0].
 * Returns:  1 = likely ascending,  -1 = likely descending,  0 = mixed.
 */
static int pdq_choose_pivot(pdq_ctx *ctx, unsigned char *base, size_t n) {
    size_t mid = n / 2;
    int swaps = 0;

    if (n >= PDQ_NINTHER_THRESHOLD) {
        size_t step = n / 8;
        swaps += pdq_sort3(ctx, base, 0,                step,          step * 2);
        swaps += pdq_sort3(ctx, base, mid - step,        mid,           mid + step);
        swaps += pdq_sort3(ctx, base, n - 1 - 2 * step, n - 1 - step,  n - 1);
        swaps += pdq_sort3(ctx, base, step,              mid,           n - 1 - step);
        pdq_swap(ctx, pdq_elem(base, 0, ctx->size),
                      pdq_elem(base, mid, ctx->size));
    } else {
        swaps += pdq_sort3(ctx, base, 0, mid, n - 1);
        /* after sort3: base[0] <= base[mid] <= base[n-1] */
        pdq_swap(ctx, pdq_elem(base, 0, ctx->size),
                      pdq_elem(base, mid, ctx->size));
        /* now base[0] = median, base[n-1] = max >= pivot */
    }

    if (swaps == 0) return 1;
    if (swaps >= 8) return -1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* heapsort fallback                                                   */
/* ------------------------------------------------------------------ */

static void pdq_sift_down(pdq_ctx *ctx, unsigned char *base, size_t start, size_t end) {
    size_t root = start;
    for (;;) {
        size_t child = root * 2 + 1;
        if (child >= end) break;
        size_t max_idx = root;
        if (ctx->cmp(pdq_elem(base, max_idx, ctx->size),
                     pdq_elem(base, child,   ctx->size)) < 0)
            max_idx = child;
        if (child + 1 < end &&
            ctx->cmp(pdq_elem(base, max_idx,   ctx->size),
                     pdq_elem(base, child + 1, ctx->size)) < 0)
            max_idx = child + 1;
        if (max_idx == root) break;
        pdq_swap(ctx, pdq_elem(base, root, ctx->size),
                      pdq_elem(base, max_idx, ctx->size));
        root = max_idx;
    }
}

static void pdq_heapsort(pdq_ctx *ctx, unsigned char *base, size_t n) {
    if (n < 2) return;
    for (size_t i = n / 2; i > 0; --i)
        pdq_sift_down(ctx, base, i - 1, n);
    for (size_t end = n; end > 1; --end) {
        pdq_swap(ctx, pdq_elem(base, 0, ctx->size),
                      pdq_elem(base, end - 1, ctx->size));
        pdq_sift_down(ctx, base, 0, end - 1);
    }
}

/* ------------------------------------------------------------------ */
/* pattern breaking                                                    */
/* ------------------------------------------------------------------ */

static uint32_t pdq_xorshift32(uint32_t *state) {
    uint32_t x = *state;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    *state = x;
    return x;
}

static void pdq_break_patterns(pdq_ctx *ctx, unsigned char *base, size_t n) {
    if (n < 8) return;
    uint32_t rnd = (uint32_t)n;
    size_t half = n / 2, q1 = n / 4, q3 = q1 + half;
    size_t a = q1 + (pdq_xorshift32(&rnd) % (half ? half : 1));
    size_t b = q1 + (pdq_xorshift32(&rnd) % (half ? half : 1));
    size_t c = q1 + (pdq_xorshift32(&rnd) % (half ? half : 1));
    pdq_swap(ctx, pdq_elem(base, q1,   ctx->size), pdq_elem(base, a, ctx->size));
    pdq_swap(ctx, pdq_elem(base, half, ctx->size), pdq_elem(base, b, ctx->size));
    pdq_swap(ctx, pdq_elem(base, q3,   ctx->size), pdq_elem(base, c, ctx->size));
}

/* ------------------------------------------------------------------ */
/* partition_right: clean Hoare partition (NO extra pass)              */
/*                                                                     */
/* Pivot at base[0] and ctx->pivot.                                    */
/* Result: base[0..pp) < pivot, base[pp] = pivot, base[pp+1..n) >= pv */
/* ------------------------------------------------------------------ */

static size_t pdq_partition_right(pdq_ctx *ctx, unsigned char *base,
                                  size_t n, int *already_partitioned) {
    const size_t size = ctx->size;
    unsigned char *pivot = ctx->pivot;

    /* pointer-based scans: eliminate index*size multiplications in hot loop */
    unsigned char *lo = base + size;               /* skip pivot at base[0] */
    unsigned char *hi = base + (n - 1) * size;

    /* initial guarded scans */
    while (lo <= hi && ctx->cmp(lo, pivot) < 0)  lo += size;
    while (lo <= hi && ctx->cmp(hi, pivot) >= 0) hi -= size;

    *already_partitioned = (lo > hi);

    while (lo < hi) {
        pdq_swap(ctx, lo, hi);
        /* after swap: sentinels guaranteed for inner scans */
        do { lo += size; } while (ctx->cmp(lo, pivot) < 0);
        do { hi -= size; } while (ctx->cmp(hi, pivot) >= 0);
    }

    /* place pivot at partition point */
    unsigned char *pp = lo - size;
    if (pp > base) memcpy(base, pp, size);
    memcpy(pp, pivot, size);

    return (size_t)(pp - base) / size;
}

/* ------------------------------------------------------------------ */
/* partition_left: for equal-element optimization                      */
/*                                                                     */
/* Result: base[0..pp) <= pivot, base[pp] = pivot, base[pp+1..n) > pv */
/* Used when all elements are suspected equal (skips equal block).     */
/* ------------------------------------------------------------------ */

static size_t pdq_partition_left(pdq_ctx *ctx, unsigned char *base, size_t n) {
    const size_t size = ctx->size;
    unsigned char *pivot = ctx->pivot;
    memcpy(pivot, base, size);

    unsigned char *lo = base + size;
    unsigned char *hi = base + (n - 1) * size;

    /* right scan: skip > pivot, stop at <= pivot */
    while (lo <= hi && ctx->cmp(pivot, hi) < 0) hi -= size;
    /* left scan: skip <= pivot, stop at > pivot */
    while (lo <= hi && ctx->cmp(lo, pivot) <= 0) lo += size;

    while (lo < hi) {
        pdq_swap(ctx, lo, hi);
        do { lo += size; } while (ctx->cmp(lo, pivot) <= 0);
        do { hi -= size; } while (ctx->cmp(pivot, hi) < 0);
    }

    /* base[0..hi] <= pivot, base[hi+1..n-1] > pivot */
    if (hi > base) memcpy(base, hi, size);
    memcpy(hi, pivot, size);

    return (size_t)(hi - base) / size;
}

/* ------------------------------------------------------------------ */
/* core loop                                                           */
/* ------------------------------------------------------------------ */

static void pdq_sort_loop(pdq_ctx *ctx, unsigned char *base, size_t n,
                          size_t bad_allowed, int leftmost) {
    for (;;) {
        if (n <= PDQ_INSERTION_SORT_THRESHOLD) {
            if (leftmost)
                pdq_insertion_sort(ctx, base, n);
            else
                pdq_unguarded_insertion_sort(ctx, base, n);
            return;
        }

        if (bad_allowed == 0) {
            pdq_heapsort(ctx, base, n);
            return;
        }

        /* equal-element fast path (non-leftmost only):
         * if element before this partition >= first element,
         * all elements here are likely equal to the parent pivot. */
        if (!leftmost) {
            if (ctx->cmp(base - ctx->size, base) >= 0) {
                /* partition_left groups equals at the left, > pivot at the right */
                size_t pp = pdq_partition_left(ctx, base, n);
                /* skip past the equal block */
                base = pdq_elem(base, pp + 1, ctx->size);
                n = n - pp - 1;
                continue;
            }
        }

        pdq_choose_pivot(ctx, base, n);
        memcpy(ctx->pivot, base, ctx->size);

        int already_partitioned = 0;
        size_t pp = pdq_partition_right(ctx, base, n, &already_partitioned);

        size_t left_size  = pp;
        size_t right_size = n - pp - 1;
        int highly_unbalanced = (left_size < n / 8 || right_size < n / 8);

        if (highly_unbalanced) {
            --bad_allowed;
            if (left_size >= PDQ_INSERTION_SORT_THRESHOLD)
                pdq_break_patterns(ctx, base, left_size);
            if (right_size >= PDQ_INSERTION_SORT_THRESHOLD)
                pdq_break_patterns(ctx, base + (pp + 1) * ctx->size, right_size);
        } else if (already_partitioned) {
            int left_ok  = pdq_partial_insertion_sort(ctx, base, left_size);
            int right_ok = pdq_partial_insertion_sort(
                ctx, base + (pp + 1) * ctx->size, right_size);
            if (left_ok && right_ok) return;
        }

        /* recurse on smaller partition, loop on larger (tail-call elimination) */
        unsigned char *right_base = base + (pp + 1) * ctx->size;
        if (left_size < right_size) {
            pdq_sort_loop(ctx, base, left_size, bad_allowed, leftmost);
            base = right_base;
            n = right_size;
            leftmost = 0;
        } else {
            pdq_sort_loop(ctx, right_base, right_size, bad_allowed, 0);
            n = left_size;
        }
    }
}

/* ------------------------------------------------------------------ */
/* public API                                                          */
/* ------------------------------------------------------------------ */

void pdq_qsort(void *base, size_t nmemb, size_t size,
               int (*compar)(const void *, const void *)) {
    if (base == NULL || compar == NULL || size == 0 || nmemb < 2) return;

    unsigned char *scratch = (unsigned char *)malloc(size * 2);
    if (!scratch) return;

    pdq_ctx ctx;
    ctx.size      = size;
    ctx.cmp       = compar;
    ctx.swap_mode = pdq_choose_swap_mode(base, size);
    ctx.tmp       = scratch;
    ctx.pivot     = scratch + size;

    size_t bad_allowed = pdq_floor_log2(nmemb) * 2 + 1;
    pdq_sort_loop(&ctx, (unsigned char *)base, nmemb, bad_allowed, 1);

    free(scratch);
}
