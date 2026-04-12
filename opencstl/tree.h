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
#if !defined(_OPENCSTL_TREE_H)
#define _OPENCSTL_TREE_H


#include"error.h"
#include"zalloc.h"

#define P	    (-4)
#define R       (-1)
#define L       (-2)
#define RED     ((size_t)1)
#define BLACK   ((size_t)0)
// _(N,V): raw size_t lvalue, no cast, works on all compilers including Windows Clang.
// Write sites: _(N,V) = (size_t)val
// Read sites:  (void*)_(N,V)
#define _(N,V)	OPENCSTL_NIDX(&N, V)
#define COLOR(N)	_(N, -5)




#define CSTL_ARENA_CHUNK_SIZE 256
typedef struct cstl_arena_chunk cstl_arena_chunk;

struct cstl_arena_chunk {
    cstl_arena_chunk *next;
    size_t used;
    size_t capacity;
    size_t node_size;
};

OPENCSTL_FUNC cstl_arena_chunk *__cstl_arena_new_chunk(size_t node_size, size_t capacity) {
    cstl_arena_chunk *chunk = (cstl_arena_chunk *) zalloc(
        sizeof(cstl_arena_chunk) + node_size * capacity
    );
    chunk->next = NULL;
    chunk->used = 0;
    chunk->capacity = capacity;
    chunk->node_size = node_size;
    return chunk;
}


OPENCSTL_FUNC void *__cstl_arena_alloc(cstl_arena_chunk **arena, void **freelist, size_t node_size) {
    if (*freelist != NULL) {
        void *reused = *freelist;
        *freelist = *(void **) reused;
        memset(reused, 0, node_size);
        return reused;
    }

    if (*arena == NULL || (*arena)->used >= (*arena)->capacity) {
        cstl_arena_chunk *new_chunk = __cstl_arena_new_chunk(node_size, CSTL_ARENA_CHUNK_SIZE);
        new_chunk->next = *arena;
        *arena = new_chunk;
    }

    void *ptr = (char *) (*arena + 1) + (*arena)->node_size * (*arena)->used;
    memset(ptr, 0, node_size);
    (*arena)->used++;
    return ptr;
}

OPENCSTL_FUNC void __cstl_arena_dealloc(void **freelist, void *raw_ptr) {
    *(void **) raw_ptr = *freelist;
    *freelist = raw_ptr;
}

OPENCSTL_FUNC void __cstl_arena_free_all(cstl_arena_chunk **arena, void **freelist) {
    cstl_arena_chunk *c = *arena;
    while (c != NULL) {
        cstl_arena_chunk *next = c->next;
        zfree(c);
        c = next;
    }
    *arena = NULL;
    *freelist = NULL;
}

SELECT_ANY char nil_buffer[sizeof(void *) * NIDX_TREE_NODE_SIZE] = {0};
SELECT_ANY void *nil = NULL;
OPENCSTL_FUNC void *__cstl_tree_node(size_t type_size, size_t node_type) {
    // [color][parent][node type][left][right] -> [data]
    size_t node_sz = type_size + sizeof(void *) * NIDX_TREE_NODE_SIZE;
    node_sz = (node_sz + sizeof(void *) - 1) & ~(sizeof(void *) - 1);
    void *ptr = (char *) zalloc(node_sz, 1) + sizeof(void *) * NIDX_TREE_NODE_SIZE;
    OPENCSTL_NIDX(&ptr, -3) = node_type;
    COLOR(ptr) = BLACK;
    return ptr;
}

OPENCSTL_FUNC void *__cstl_tree_node_pooled(void **container, size_t type_size, size_t node_type) {
    // [color][parent][node type][left][right] -> [data]
    size_t raw_node_sz = type_size + sizeof(void *) * NIDX_TREE_NODE_SIZE;
    raw_node_sz = (raw_node_sz + sizeof(void *) - 1) & ~(sizeof(void *) - 1);


    cstl_arena_chunk **arena = (cstl_arena_chunk **) &OPENCSTL_NIDX(container, -7);
    void **freelist = (void **) &OPENCSTL_NIDX(container, -6);

    char *raw = (char *) __cstl_arena_alloc(arena, freelist, raw_node_sz);
    void *ptr = raw + sizeof(void *) * NIDX_TREE_NODE_SIZE;
    OPENCSTL_NIDX(&ptr, -3) = (size_t) node_type;
    COLOR(ptr) = BLACK;
    return ptr;
}

// ░██████╗███████╗████████╗
// ██╔════╝██╔════╝╚══██╔══╝
// ╚█████╗░█████╗░░░░░██║░░░
// ░╚═══██╗██╔══╝░░░░░██║░░░
// ██████╔╝███████╗░░░██║░░░
// ╚═════╝░╚══════╝░░░╚═╝░░░

#define cstl_set         _cstl_set
#define _cstl_set(KEY, ...)	    _CSTL_SET_DISPATCH(KEY, ##__VA_ARGS__, NULL)
#define _CSTL_SET_DISPATCH(KEY, COMP, ...) __cstl_set(sizeof(KEY),#KEY,(void*)(COMP))


OPENCSTL_FUNC void *__cstl_set(size_t key_size, char *type_key, void *compare) {
    if (nil == NULL) {
        nil = nil_buffer + sizeof(void *) * NIDX_TREE_NODE_SIZE;
        _(nil, -1) = _(nil, -2) = _(nil, -4) = (size_t) nil;
    }
    size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
    void *ptr = (char *) zalloc(header_sz + sizeof(size_t)) + header_sz;
    void **container = &ptr;
    OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_SET;
    OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(container, NIDX_TSIZE) = key_size;
    OPENCSTL_NIDX(container, -9) = 0;
    OPENCSTL_NIDX(container, -8) = !strcmp(type_key, "float");
    OPENCSTL_NIDX(container, -7) = 0;
    OPENCSTL_NIDX(container, -6) = 0;
    OPENCSTL_NIDX(container, -4) = 0; //value size, but set does not have value.
    OPENCSTL_NIDX(container, -3) = (size_t) type_key; //type
    OPENCSTL_NIDX(container, -2) = (size_t) compare; //compare function
    OPENCSTL_NIDX(container, -1) = 0;
    OPENCSTL_NIDX(container, 0) = (size_t) nil; //root
    return ptr;
}

// ███╗░░░███╗░█████╗░██████╗░
// ████╗░████║██╔══██╗██╔══██╗
// ██╔████╔██║███████║██████╔╝
// ██║╚██╔╝██║██╔══██║██╔═══╝░
// ██║░╚═╝░██║██║░░██║██║░░░░░
// ╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝░░░░░
#define cstl_map         _cstl_map
#define _cstl_map(KEY, VALUE, ...)	_CSTL_MAP_DISPATCH(KEY, VALUE, ##__VA_ARGS__, NULL)
#define _CSTL_MAP_DISPATCH(KEY, VALUE, COMP, ...) __cstl_map(sizeof(KEY), sizeof(VALUE), #KEY, #VALUE, (void*)(COMP))
OPENCSTL_FUNC void *__cstl_map(size_t key_size, size_t value_size, char *type_key, char *type_value, void *compare) {
    if (nil == NULL) {
        nil = nil_buffer + sizeof(void *) * 5;
        _(nil, -1) = _(nil, -2) = _(nil, -4) = (size_t) nil;
    }
    size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
    void *ptr = (char *) zalloc(header_sz + sizeof(size_t)) + header_sz;
    void **container = &ptr;
    OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_MAP;
    OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(container, NIDX_TSIZE) = key_size;
    OPENCSTL_NIDX(container, -9) = !strcmp(type_value, "float");
    OPENCSTL_NIDX(container, -8) = !strcmp(type_key, "float");
    OPENCSTL_NIDX(container, -7) = 0;
    OPENCSTL_NIDX(container, -6) = 0;
    OPENCSTL_NIDX(container, -5) = (size_t) type_value; //not-reserved
    OPENCSTL_NIDX(container, -4) = value_size; //value size
    OPENCSTL_NIDX(container, -3) = (size_t) type_key; //not-reserved
    OPENCSTL_NIDX(container, -2) = (size_t) compare; //compare function
    OPENCSTL_NIDX(container, -1) = 0;
    OPENCSTL_NIDX(container, 0) = (size_t) nil; //root
    return ptr;
}

OPENCSTL_FUNC void __cstl_tree_left_rotate(void **container, void *x) {
    void ***root = (void ***) *container;
    void *y = (void *) _(x, R);
    _(x, R) = _(y, L);
    if ((void *) _(y, L) != nil) {
        _(_(y, L), P) = (size_t) x;
    }
    _(y, P) = _(x, P);
    if ((void *) _(x, P) == nil) {
        *root = y;
    } else if (x == (void *) _(_(x,P), L)) {
        _(_(x, P), L) = (size_t) y;
    } else {
        _(_(x, P), R) = (size_t) y;
    }
    _(y, L) = (size_t) x;
    _(x, P) = (size_t) y;
}

OPENCSTL_FUNC void __cstl_tree_right_rotate(void **container, void *x) {
    void ***root = (void ***) *container;
    void *y = (void *) _(x, L);
    _(x, L) = _(y, R);
    if ((void *) _(y, R) != nil) {
        _(_(y, R), P) = (size_t) x;
    }
    _(y, P) = _(x, P);
    if ((void *) _(x, P) == nil) {
        *root = y;
    } else if (x == (void *) _(_(x, P), R)) {
        _(_(x, P), R) = (size_t) y;
    } else {
        _(_(x, P), L) = (size_t) y;
    }
    _(y, R) = (size_t) x;
    _(x, P) = (size_t) y;
}

OPENCSTL_FUNC void __cstl_tree_insert_fixup(void **container, void *z) {
    while (COLOR(_(z, P)) == RED) {
        if ((void *) _(z, P) == (void *) _(_(_(z, P), P), L)) {
            void *y = (void *) _(_(_(z, P), P), R);
            if (COLOR(y) == RED) {
                COLOR(_(z, P)) = BLACK;
                COLOR(y) = BLACK;
                COLOR(_(_(z, P), P)) = RED;
                z = (void *) _(_(z, P), P);
            } else {
                if (z == (void *) _(_(z, P), R)) {
                    z = (void *) _(z, P);
                    __cstl_tree_left_rotate(container, z);
                }
                COLOR(_(z, P)) = BLACK;
                COLOR(_(_(z, P),P)) = RED;
                __cstl_tree_right_rotate(container, (void *) _(_(z, P), P));
            }
        } else {
            void *y = (void *) _(_(_(z, P), P), L);
            if (COLOR(y) == RED) {
                COLOR(_(z, P)) = BLACK;
                COLOR(y) = BLACK;
                COLOR(_(_(z, P), P)) = RED;
                z = (void *) _(_(z, P), P);
            } else {
                if (z == (void *) _(_(z, P), L)) {
                    z = (void *) _(z, P);
                    __cstl_tree_right_rotate(container, z);
                }
                COLOR(_(z, P)) = BLACK;
                COLOR(_(_(z, P), P)) = RED;
                __cstl_tree_left_rotate(container, (void *) _(_(z, P), P));
            }
        }
    }
    void ***root = (void ***) *container;
    COLOR(*root) = BLACK;
}

OPENCSTL_FUNC void __cstl_tree_insert(void **container, void *key, void *value) {
    size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    //size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    size_t type_size = key_size + value_size;
    cstl_compare compare = (cstl_compare) OPENCSTL_NIDX(container, -2);

    // char *type_key = (char *) OPENCSTL_NIDX(container, -3);
    // char *type_value = (char *) OPENCSTL_NIDX(container, -5);

    

#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t is_float_value = OPENCSTL_NIDX(container, -9);
    float keyf = 0.0F;
    if (is_float_key) {
        keyf = (float) *(double *) key;
        key = &keyf;
    }

    float valuef = 0.0F;
    if (is_float_value) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif

    void ***root = (void ***) *container;
    //void *n = __cstl_tree_node(type_size, container_type);
    void *n = __cstl_tree_node_pooled(container, type_size, container_type);
    memcpy(n, key, key_size);
    if (value) {
        memcpy((char *) n + key_size, value, value_size);
    }
    void *p = nil;
    while (*root != nil) {

        p = *root;
        int r = compare ? compare(*root, n) : memcmp(*root, n, type_size);
        if (r == 0) {
            return;
        } else if (r > 0) {
            //to left
            root = (void ***) &OPENCSTL_NIDX(root, L);
        } else {
            //to right
            root = (void ***) &OPENCSTL_NIDX(root, R);
        }
    }
    OPENCSTL_NIDX(&n, P) = (size_t) p;
    OPENCSTL_NIDX(&n, L) = (size_t) nil;
    OPENCSTL_NIDX(&n, R) = (size_t) nil;
    COLOR(n) = RED;

    *root = (void **) n;
    __cstl_tree_insert_fixup(container, n);

    OPENCSTL_NIDX(container, -1) = (OPENCSTL_NIDX(container, -1)) + 1;
}

OPENCSTL_FUNC void __cstl_tree_transplant(void **container, void *u, void *v) {
    void ***root = (void ***) *container;
    if ((void *) _(u, P) == nil) {
        *root = v;
    } else if (u == (void *) _(_(u, P), L)) {
        _(_(u, P), L) = (size_t) v;
    } else {
        _(_(u, P), R) = (size_t) v;
    }
    _(v, P) = _(u, P);
}

OPENCSTL_FUNC void *__cstl_tree_toleft(void *n) {
    while ((void *) _(n, L) != nil) {
        n = (void *) _(n, L);
    }
    return n;
}

OPENCSTL_FUNC void *__cstl_tree_toright(void *n) {
    while ((void *) _(n, R) != nil) {
        n = (void *) _(n, R);
    }
    return n;
}

OPENCSTL_FUNC void __cstl_tree_erase_fixup(void **container, void *x) {
    void ***root = (void ***) *container;
    while (x != *root && COLOR(x) == BLACK) {
        intmax_t expression = (x == (void *) _(_(x, P), L));
        intmax_t left = expression ? L : R;
        intmax_t right = expression ? R : L;
        void (*func[2])(void **, void *) = {__cstl_tree_left_rotate, __cstl_tree_right_rotate};
        if (!expression) {
            void (*tmp)(void **, void *) = func[0];
            func[0] = func[1];
            func[1] = tmp;
        }
        void *w = (void *) _(_(x, P), right);
        if (COLOR(w) == RED) {
            COLOR(w) = BLACK;
            COLOR(_(x, P)) = RED;
            func[0](container, (void *) _(x, P));
            w = (void *) _(_(x, P), right);
        }
        if (COLOR(_(w, left)) == BLACK && COLOR(_(w, right)) == BLACK) {
            COLOR(w) = RED;
            x = (void *) _(x, P);
        } else {
            if (COLOR(_(w, right)) == BLACK) {
                COLOR(_(w, left)) = BLACK;
                COLOR(w) = RED;
                func[1](container, w);
                w = (void *) _(_(x, P), right);
            }
            COLOR(w) = COLOR(_(x, P));
            COLOR(_(x, P)) = BLACK;
            COLOR(_(w, right)) = BLACK;
            func[0](container, (void *) _(x, P));
            x = *root;
        }
    }
    COLOR(x) = BLACK;
}

OPENCSTL_FUNC void __cstl_tree_erase(void **container, void **iter) {
    if (iter == NULL)return;
    // size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    // size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    // size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    // size_t value_size = OPENCSTL_NIDX(container, -4);
    //size_t type_size = key_size + value_size;
    //cstl_compare compare = (cstl_compare) OPENCSTL_NIDX(container, -2);
    //void ***root = (void ***) *container;
    void *z = iter;

    void *y = z;
    void *x = NULL;
    size_t y_original_color = (size_t) COLOR(y);
    if ((void *) _(z, L) == nil) {
        x = (void *) _(z, R);
        __cstl_tree_transplant(container, z, (void *) _(z, R));
    } else if ((void *) _(z, R) == nil) {
        x = (void *) _(z, L);
        __cstl_tree_transplant(container, z, (void *) _(z, L));
    } else {
        y = __cstl_tree_toleft((void *) _(z, R));
        y_original_color = (size_t) COLOR(y);
        x = (void *) _(y, R);
        if ((void *) _(y, P) == z) {
            _(x, P) = (size_t) y;
        } else {
            __cstl_tree_transplant(container, y, (void *) _(y, R));
            _(y, R) = _(z, R);
            _(_(y, R), P) = (size_t) y;
        }
        __cstl_tree_transplant(container, z, y);
        _(y, L) = _(z, L);
        _(_(y, L), P) = (size_t) y;
        COLOR(y) = COLOR(z);
    }
    if (y_original_color == (size_t) BLACK) {
        __cstl_tree_erase_fixup(container, x);
    }
    //free(&OPENCSTL_NIDX(&iter, -5));
    {
        void **freelist = (void **) &OPENCSTL_NIDX(container, -6);
        __cstl_arena_dealloc(freelist, &OPENCSTL_NIDX(&iter, -5));
        OPENCSTL_NIDX(container, -1) = (OPENCSTL_NIDX(container, -1)) - 1;
    }
}

OPENCSTL_FUNC void *__cstl_tree_find(void **container, void *key) {
    // size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    // size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    size_t type_size = key_size + value_size;
    //char *type_key = (char *) OPENCSTL_NIDX(container, -3);

    

#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    float keyf = 0.0F;
    if (is_float_key) {
        keyf = (float) *(double *) key;
        key = &keyf;
    }
#endif


    cstl_compare compare = (cstl_compare) OPENCSTL_NIDX(container, -2);
    void ***root = (void ***) *container;
    while (*root != nil) {
        int r = compare ? compare(*root, key) : memcmp(*root, key, type_size);
        if (r == 0) {
            return *root;
        } else if (r > 0) {
            //to left
            root = (void ***) &OPENCSTL_NIDX(root, L);
        } else {
            //to right
            root = (void ***) &OPENCSTL_NIDX(root, R);
        }
    }
    return NULL;
}

OPENCSTL_FUNC void *__cstl_tree_begin(void **container) {
    void ***root = (void ***) *container;
    return *root != nil ? __cstl_tree_toleft(*root) : nil;
}

OPENCSTL_FUNC void *__cstl_tree_rbegin(void **container) {
    void ***root = (void ***) *container;
    return *root != nil ? __cstl_tree_toright(*root) : nil;
}

OPENCSTL_FUNC void *__cstl_tree_end_rend(void **container) {
    (void)container;
    return nil;
}

// OPENCSTL_FUNC void __cstl_tree_clear(void **container) {
//     size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
//     size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
//     size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
//     size_t value_size = OPENCSTL_NIDX(container, -4);
//     size_t type_size = key_size + value_size;
//     cstl_compare compare = (cstl_compare) OPENCSTL_NIDX(container, -2);
//     void ***root = (void ***) *container;
//
//     void *c = *root;
//     while (c != nil) {
//         if ((void *) _(c, R) != nil) {
//             void *m = __cstl_tree_toleft(c);
//             _(m, L) = _(c, R);
//             _(_(c, R), P) = _(m, L);
//         }
//         void *t = c;
//         c = (void *) _(c, L);
//         free(&OPENCSTL_NIDX(&t, -5));
//
//     }
//     *root = nil;
// }

OPENCSTL_FUNC void __cstl_tree_clear(void **container) {
    cstl_arena_chunk **arena = (cstl_arena_chunk **) &OPENCSTL_NIDX(container, -7);
    void **freelist = (void **) &OPENCSTL_NIDX(container, -6);
    void ***root = (void ***) *container;
    
    __cstl_arena_free_all(arena, freelist);
    OPENCSTL_NIDX(container, -1) = 0;
    *root = nil;
}

OPENCSTL_FUNC void __cstl_tree_free(void **container) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    __cstl_tree_clear(container);
    zfree((char *) (*container) - header_sz);
    *container = NULL;
}

OPENCSTL_FUNC void *__cstl_tree_next_prev(void *it, int r, int l, void *(todeep)(void *)) {
    //next = r(-1) , l(-2)
    //prev = r(-2), l(-1)
    if ((void *) _(it, r) != nil) {
        it = todeep((void *) _(it, r));
    } else {
        if ((void *) _(_(it, P), l) == it) {
            it = (void *) _(it, P);
        } else {
            while ((void *) _(_(it, P), r) == it) {
                it = (void *) _(it, P);
            }
            it = (void *) _(it, P);
        }
    }
    return it;
}

// OPENCSTL_FUNC size_type ___cstl_tree_size(void *n) {
//     if (n == nil)return 0;
//     return ___cstl_tree_size((void *) _(n, L)) + ___cstl_tree_size((void *) _(n, R)) + 1;
// }

OPENCSTL_FUNC size_type __cstl_tree_size(void **container) {
    //size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    //size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    //size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    //size_t value_size = OPENCSTL_NIDX(container, -4);
    //size_t type_size = key_size + value_size;
    //cstl_compare compare = (cstl_compare) OPENCSTL_NIDX(container, -2);
    //void ***root = (void ***) *container;
    //void *c = *root;
    size_t length = OPENCSTL_NIDX(container, -1);
    //return ___cstl_tree_size(c);
    return (size_type)length;
}
#undef P
#undef L
#undef R
#undef _
#undef RED
#undef BLACK


#endif
