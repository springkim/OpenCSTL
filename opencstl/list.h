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
//               Copyright (C) 2018-2026, Kim Bomm, all rights reserved.
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
#if !defined(_OPENCSTL_LIST_H)
#define _OPENCSTL_LIST_H
#include"error.h"
#include"zalloc.h"
#define cstl_list(TYPE)		__cstl_list(sizeof(TYPE),#TYPE)
#define NTAIL(N)	(N==-1?-2:N)
OPENCSTL_FUNC void *__cstl_list(size_t type_size, char *type) {
    size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
    void *block = calloc(header_sz + sizeof(size_t), 1);
    if (block == NULL) {
        cstl_error("Failed to allocate memory for list");
    }
    void *ptr = ((char *) block) + header_sz;
    void **container = &ptr;
    OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_LIST;
    OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(container, NIDX_TSIZE) = type_size;
    OPENCSTL_NIDX(container, -8) = !strcmp(type, "float");
    OPENCSTL_NIDX(container, -4) = (size_t) type;
    OPENCSTL_NIDX(container, -2) = 0; //tail
    OPENCSTL_NIDX(container, -1) = 0; //length
    OPENCSTL_NIDX(container, 0) = 0; //head
    bool iveb_init = false;
    if (iveb == NULL) {
        iveb = iveb_new();
        iveb_init = true;
    }
    iveb_insert(iveb, ptr, (char *) ptr + sizeof(size_t), CT_LIST, type_size, type);
    if (iveb_init) {
        atexit(__opencstl_iveb_destroy);
    }
    return ptr;
}

OPENCSTL_FUNC void *__cstl_list_node(size_t type_size) {
    //[node type][prev][next] ↘ [data]
    size_t header_sz = sizeof(void *) * NIDX_LIST_NODE_SIZE;
    size_t node_sz = type_size + header_sz;
    void *ptr = (char *) calloc(node_sz, 1) + header_sz;
    void **node = &ptr;
    OPENCSTL_NIDX(node, -3) = OPENCSTL_LIST;
    return ptr;
}

OPENCSTL_FUNC void __cstl_list_push_back_front(void **container, void *value, int ntail, int nhead) {
    //size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    void **tail = (void **) &OPENCSTL_NIDX(container, NTAIL(ntail)); //-1 , 0
    void **head = (void **) &OPENCSTL_NIDX(container, nhead); //0  , -1
    void *n = __cstl_list_node(type_size);
    //char *type = (char *) OPENCSTL_NIDX(container, -4);

#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float = OPENCSTL_NIDX(container, -8);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    if (value)
        memcpy(n, value, type_size);
    if (*head == NULL && *tail == NULL) {
        *head = *tail = n;
    } else {
        OPENCSTL_NIDX(tail, -(ntail + 2)) = (size_t) n;
        OPENCSTL_NIDX(&n, -(nhead + 2)) = (size_t) *tail;
        *tail = n;
    }
    OPENCSTL_NIDX(container, -1)++;
}

OPENCSTL_FUNC void __cstl_list_pop_back_front(void **container, int ntail, int nhead) {
    //size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    //size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    void **tail = (void **) &OPENCSTL_NIDX(container, NTAIL(ntail)); //-1 , 0
    void **head = (void **) &OPENCSTL_NIDX(container, nhead); //0  , -1
    if (*head == NULL || *tail == NULL) {
        cstl_error("No elements in cstl_list");
    }
    if (*head == *tail) {
        free(&OPENCSTL_NIDX(tail, -3)); //fix
        *head = *tail = 0;
    } else {
        *tail = (void *) OPENCSTL_NIDX(tail, -(nhead + 2));
        void *fb = (void *) OPENCSTL_NIDX(tail, -(ntail + 2));
        free(&OPENCSTL_NIDX(&fb, -3));
        OPENCSTL_NIDX(tail, -(ntail + 2)) = 0;
    }

    OPENCSTL_NIDX(container, -1)--;
}

OPENCSTL_FUNC void *__cstl_list_next_prev(void *it, int n) {
    //next(-1), prev(-2)
    return (void *) OPENCSTL_NIDX(&it, n);
}

OPENCSTL_FUNC size_type __cstl_list_size(void **container) {
    return (size_type) OPENCSTL_NIDX(container, -1);
}

OPENCSTL_FUNC void __cstl_list_resize(void **container, size_t n, void *value) {
    size_t length = OPENCSTL_NIDX(container, -1);
    if (n < length) {
        while (n < length) {
            __cstl_list_pop_back_front((void **) container, -1, 0);
            length--;
        }
    } else {
        while (n >= length) {
            __cstl_list_push_back_front((void **) container, value, -1, 0);
            length++;
        }
    }
    OPENCSTL_NIDX(container, -1) = n;
}

OPENCSTL_FUNC void __cstl_list_insert(void **container, void **iter, size_t N, void *value) {
    //size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    void **tail = (void **) &OPENCSTL_NIDX(container, -2);
    void **head = (void **) &OPENCSTL_NIDX(container, 0);
    //char *type = (char *) OPENCSTL_NIDX(container, -4);

#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float = OPENCSTL_NIDX(container, -8);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    void *nhead = __cstl_list_node(type_size);
    memcpy(nhead, value, type_size);
    void *ntail = nhead;
    for (size_t i = 1; i < N; i++) {
        void *n = __cstl_list_node(type_size);
        memcpy(n, value, type_size);
        OPENCSTL_NIDX(&n, -2) = (size_t) ntail; //n->prev=tail
        OPENCSTL_NIDX(&ntail, -1) = (size_t) n; //tail->next=n;
        ntail = n;
    }
    OPENCSTL_NIDX(&ntail, -1) = (size_t) *iter; //n->next=iter
    if (*head == NULL && *tail == NULL) {
        *head = nhead;
        *tail = ntail;
    } else if (*iter != NULL) {
        if (OPENCSTL_NIDX(iter, -2) != 0) {
            OPENCSTL_NIDX(&(OPENCSTL_NIDX(iter, -2)), -1) = (size_t) nhead; //iter->prev->next=n;
        }
        OPENCSTL_NIDX(&nhead, -2) = OPENCSTL_NIDX(iter, -2); //n->prev=iter->prev
        OPENCSTL_NIDX(iter, -2) = (size_t) ntail; //iter->prev=n;
        if (*iter == *head) {
            *head = nhead;
        }
    } else {
        OPENCSTL_NIDX(tail, -1) = (size_t) nhead; //tail->next=n;
        OPENCSTL_NIDX(&nhead, -2) = (size_t) *tail; //n->prev=tail;
        *tail = ntail;
    }
    OPENCSTL_NIDX(container, -1) += N;
}

OPENCSTL_FUNC void __cstl_list_erase(void **container, void **iter_begin, void **iter_end) {
    //size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    //size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    void **tail = (void **) &OPENCSTL_NIDX(container, -2);
    void **head = (void **) &OPENCSTL_NIDX(container, 0);
    if (*iter_begin == NULL) {
        cstl_error("iter_begin could not be NULL");
    }
    if (*iter_begin == *head) {
        *head = *iter_end;
        if (*head != NULL) {
            OPENCSTL_NIDX(head, -2) = 0;
        }
    } else {
        OPENCSTL_NIDX(&OPENCSTL_NIDX(iter_begin, -2), -1) = (size_t) *iter_end; //begin->prev->next=end;
        if (*iter_end != NULL) {
            OPENCSTL_NIDX(iter_end, -2) = OPENCSTL_NIDX(iter_begin, -2); //end->prev=begin->prev
        }
    }
    void *it = *iter_begin;
    while (it != *iter_end) {
        void *tmp = (void *) OPENCSTL_NIDX(&it, -1);
        free(&OPENCSTL_NIDX(&it, -3));
        OPENCSTL_NIDX(container, -1)--;
        it = tmp;
    }
    if (*iter_end == NULL) {
        *tail = 0;
    }
}

OPENCSTL_FUNC void *__cstl_list_begin(void **container) {
    return (void *) OPENCSTL_NIDX(container, 0);
}

OPENCSTL_FUNC void *__cstl_list_end_rend(void **container) {
    (void) container;
    return NULL;
}

OPENCSTL_FUNC void *__cstl_list_rbegin(void **container) {
    return (void *) OPENCSTL_NIDX(container, -2);
}

OPENCSTL_FUNC void __cstl_list_clear(void **container) {
    void **tail = (void **) &OPENCSTL_NIDX(container, -2);
    void **head = (void **) &OPENCSTL_NIDX(container, 0);

    void *it = *head;
    while (it != NULL) {
        void *tmp = (void *) OPENCSTL_NIDX(&it, -1);
        free(&OPENCSTL_NIDX(&it, -3));
        it = tmp;
    }
    *head = *tail = NULL;
    OPENCSTL_NIDX(container, -1) = 0;
}

OPENCSTL_FUNC void __cstl_list_free(void **container) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    __cstl_list_clear(container);
    free((char *) (*container) - header_sz);
    *container = NULL;
}

OPENCSTL_FUNC void *__cstl_list_find(void **container, void **iter_begin, void *value) {
    //size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    //void **tail = (void **) &OPENCSTL_NIDX(container, -2);
    //void **head = (void **) &OPENCSTL_NIDX(container, 0);
    //char *type = (char *) OPENCSTL_NIDX(container, -4);

#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float = OPENCSTL_NIDX(container, -8);
    float valuef = 0.0F;
    if (is_float) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif
    void *it = *iter_begin;
    while (it != NULL) {
        if (memcmp(it, value, type_size) == 0) {
            return it;
        }
        it = (void *) OPENCSTL_NIDX(&it, -1);
    }
    return NULL;
}

OPENCSTL_FUNC void __cstl_list_msort(void **container, int (*cmp)(const void *, const void *)) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    void **tail = (void **) &OPENCSTL_NIDX(container, -2);
    void **head = (void **) &OPENCSTL_NIDX(container, 0);
    char *type = (char *) OPENCSTL_NIDX(container, -4);
    size_t length = OPENCSTL_NIDX(container, -1);

    (void) header_sz;
    (void) type_size;
    (void) type;

    if (cmp == NULL) {
        cstl_error("cmp could not be NULL");
    }
    if (*head == NULL || *tail == NULL || length < 2) {
        return;
    }

    for (size_t width = 1; width < length; width <<= 1) {
        void *curr = *head;
        void *new_head = NULL;
        void *new_tail = NULL;

        while (curr != NULL) {
            void *left = curr;
            void *right = NULL;
            void *next_run = NULL;

            size_t left_count = 0;
            size_t right_count = 0;

            for (; curr != NULL && left_count < width; ++left_count) {
                curr = (void *) OPENCSTL_NIDX(&curr, -1);
            }

            right = curr;

            for (; curr != NULL && right_count < width; ++right_count) {
                curr = (void *) OPENCSTL_NIDX(&curr, -1);
            }


            next_run = curr;

            while (left_count > 0 || right_count > 0) {
                void *pick = NULL;

                if (left_count == 0) {
                    pick = right;
                    right = (void *) OPENCSTL_NIDX(&right, -1);
                    --right_count;
                } else if (right_count == 0 || right == NULL) {
                    pick = left;
                    left = (void *) OPENCSTL_NIDX(&left, -1);
                    --left_count;
                } else if (cmp(left, right) <= 0) {
                    pick = left;
                    left = (void *) OPENCSTL_NIDX(&left, -1);
                    --left_count;
                } else {
                    pick = right;
                    right = (void *) OPENCSTL_NIDX(&right, -1);
                    --right_count;
                }

                if (new_tail == NULL) {
                    new_head = pick;
                    OPENCSTL_NIDX(&pick, -2) = 0;
                } else {
                    OPENCSTL_NIDX(&new_tail, -1) = (size_t) pick;
                    OPENCSTL_NIDX(&pick, -2) = (size_t) new_tail;
                }
                new_tail = pick;
            }

            if (new_tail != NULL) {
                OPENCSTL_NIDX(&new_tail, -1) = 0;
            }

            curr = next_run;
        }

        *head = new_head;
        *tail = new_tail;
    }
}


typedef struct {
    void *low;
    void *high;
} __cstl_qsort_range;

OPENCSTL_FUNC void __cstl_list_swap_data(void *a, void *b, size_t n) {
    unsigned char buf[128];
    unsigned char *tmp = (n <= sizeof(buf)) ? buf : (unsigned char *) calloc(n, 1);
    if (tmp == NULL) {
        cstl_error("Failed to allocate memory for swap");
    }
    memcpy(tmp, a, n);
    memcpy(a, b, n);
    memcpy(b, tmp, n);
    if (tmp != buf)
        free(tmp);
}

OPENCSTL_FUNC void *__cstl_list_mid_node(void *low, void *high) {
    void *slow = low;
    void *fast = low;
    while (fast != high) {
        fast = (void *) OPENCSTL_NIDX(&fast, -1);
        if (fast == high) break;
        fast = (void *) OPENCSTL_NIDX(&fast, -1);
        slow = (void *) OPENCSTL_NIDX(&slow, -1);
    }
    return slow;
}

OPENCSTL_FUNC void __cstl_list_median_of_three(void *low, void *high, size_t type_size,
                                               int (*cmp)(const void *, const void *)) {
    void *mid = __cstl_list_mid_node(low, high);
    if (mid == low || mid == high) return;
    if (cmp(low, mid) > 0) __cstl_list_swap_data(low, mid, type_size);
    if (cmp(low, high) > 0) __cstl_list_swap_data(low, high, type_size);
    if (cmp(mid, high) > 0) __cstl_list_swap_data(mid, high, type_size);
    __cstl_list_swap_data(mid, high, type_size);
}

OPENCSTL_FUNC void *__cstl_list_partition(void *low, void *high, size_t type_size,
                                          int (*cmp)(const void *, const void *)) {
    void *i = NULL;
    void *j = low;
    while (j != high) {
        if (cmp(j, high) <= 0) {
            i = (i == NULL) ? low : (void *) OPENCSTL_NIDX(&i, -1);
            if (i != j) {
                __cstl_list_swap_data(i, j, type_size);
            }
        }
        j = (void *) OPENCSTL_NIDX(&j, -1);
    }
    i = (i == NULL) ? low : (void *) OPENCSTL_NIDX(&i, -1);
    if (i != high) {
        __cstl_list_swap_data(i, high, type_size);
    }
    return i;
}

OPENCSTL_FUNC void __cstl_list_qsort(void **container, int (*cmp)(const void *, const void *)) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    void **tail = (void **) &OPENCSTL_NIDX(container, -2);
    void **head = (void **) &OPENCSTL_NIDX(container, 0);
    char *type = (char *) OPENCSTL_NIDX(container, -4);
    size_t length = OPENCSTL_NIDX(container, -1);
    (void) header_sz;
    (void) type_size;
    (void) type;
    if (cmp == NULL) {
        cstl_error("cmp could not be NULL");
    }
    if (*head == NULL || *tail == NULL || length < 2) {
        return;
    }
    size_t stack_cap = 64;
    __cstl_qsort_range *stack = (__cstl_qsort_range *) calloc(sizeof(__cstl_qsort_range), stack_cap);
    if (stack == NULL) {
        cstl_error("Failed to allocate memory for qsort stack");
    }
    size_t top = 0;
    stack[top].low = *head;
    stack[top].high = *tail;
    top++;
    while (top > 0) {
        top--;
        void *lo = stack[top].low;
        void *hi = stack[top].high;
        if (lo == NULL || hi == NULL || lo == hi) {
            continue;
        }
        __cstl_list_median_of_three(lo, hi, type_size, cmp);
        void *pivot = __cstl_list_partition(lo, hi, type_size, cmp);
        void *left_lo = NULL;
        void *left_hi = NULL;
        if (pivot != lo) {
            void *pp = (void *) OPENCSTL_NIDX(&pivot, -2);
            if (pp != NULL) {
                left_lo = lo;
                left_hi = pp;
            }
        }
        void *right_lo = NULL;
        void *right_hi = NULL;
        if (pivot != hi) {
            void *pn = (void *) OPENCSTL_NIDX(&pivot, -1);
            if (pn != NULL) {
                right_lo = pn;
                right_hi = hi;
            }
        }
        size_t left_cnt = 0;
        size_t right_cnt = 0;
        if (left_lo != NULL) {
            void *it = left_lo;
            while (it != left_hi) {
                left_cnt++;
                it = (void *) OPENCSTL_NIDX(&it, -1);
            }
            left_cnt++;
        }
        if (right_lo != NULL) {
            void *it = right_lo;
            while (it != right_hi) {
                right_cnt++;
                it = (void *) OPENCSTL_NIDX(&it, -1);
            }
            right_cnt++;
        }
        if (left_cnt >= right_cnt) {
            if (left_lo != NULL && left_cnt >= 2) {
                stack[top].low = left_lo;
                stack[top].high = left_hi;
                top++;
            }
            if (right_lo != NULL && right_cnt >= 2) {
                stack[top].low = right_lo;
                stack[top].high = right_hi;
                top++;
            }
        } else {
            if (right_lo != NULL && right_cnt >= 2) {
                stack[top].low = right_lo;
                stack[top].high = right_hi;
                top++;
            }
            if (left_lo != NULL && left_cnt >= 2) {
                stack[top].low = left_lo;
                stack[top].high = left_hi;
                top++;
            }
        }
    }
    free(stack);
}

OPENCSTL_FUNC size_type __cstl_list_max_size(void **container) {
    return INT_MAX;
}


#endif
