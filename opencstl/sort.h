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

#if !defined(_OPENCSTL_SORT_H)
#define _OPENCSTL_SORT_H
#include <stdlib.h>
#include <string.h>
#include "crossplatform.h"
#include "msort.h"
#include "tsort.h"
#include "pdqsort.h"
#include "deque.h"
#include "pmsort.h"
#include "list.h"
#include "defines.h"

#include "compare.h"
#include "bestsort.h"
#include "rsort.h"


// ██╗░░░██╗███╗░░██╗░██████╗████████╗░█████╗░██████╗░██╗░░░░░███████╗░░░░░░░██████╗░█████╗░██████╗░████████╗
// ██║░░░██║████╗░██║██╔════╝╚══██╔══╝██╔══██╗██╔══██╗██║░░░░░██╔════╝░░░░░░██╔════╝██╔══██╗██╔══██╗╚══██╔══╝
// ██║░░░██║██╔██╗██║╚█████╗░░░░██║░░░███████║██████╦╝██║░░░░░█████╗░░░░░░░░╚█████╗░██║░░██║██████╔╝░░░██║░░░
// ██║░░░██║██║╚████║░╚═══██╗░░░██║░░░██╔══██║██╔══██╗██║░░░░░██╔══╝░░░░░░░░░╚═══██╗██║░░██║██╔══██╗░░░██║░░░
// ╚██████╔╝██║░╚███║██████╔╝░░░██║░░░██║░░██║██████╦╝███████╗███████╗█████╗██████╔╝╚█████╔╝██║░░██║░░░██║░░░
// ░╚═════╝░╚═╝░░╚══╝╚═════╝░░░░╚═╝░░░╚═╝░░╚═╝╚═════╝░╚══════╝╚══════╝╚════╝╚═════╝░░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░

#define _CSTL_SORT_EXPAND(x) x
#define _cstl_sort_func(...) _CSTL_SORT_EXPAND(_CSTL_SORT_DISPATCH(__VA_ARGS__, NULL, NULL))
#define _CSTL_SORT_DISPATCH(CONTAINER, FUNC, ...) _cstl_sort(CONTAINER, (void*)(FUNC))
OPENCSTL_FUNC void _cstl_sort(void *container, void *_cmp) {
    ptrdiff_t distance = 0;
    size_type64 container_type = __opencstl_container_type((void **) &container, &distance);
    if (container_type == 0) return;

    switch (container_type) {
        case OPENCSTL_VECTOR: {
            size_type64 type_size = (size_type64) OPENCSTL_NIDX(&container, NIDX_TSIZE);
            char *type_name = (char *) OPENCSTL_NIDX(&container, -4);
            size_type64 length = (size_type64) OPENCSTL_NIDX(&container, -1);
            CSTL_COMPARE cmp = (CSTL_COMPARE) _cmp;
            if (cmp == NULL) {
                cmp = CSTL_LESS(type_name);
            }
            if (cmp == NULL) {
                cmp = _memcmp_funcs[type_size];
            }
            if (cmp == NULL) {
                yikes("Compare function is NULL");
            }

            cstl_unstable_sort(container, length, type_size, cmp);
        }
        break;
        case OPENCSTL_LIST: {
            size_type64 type_size = (size_type64) OPENCSTL_NIDX(&container, NIDX_TSIZE);
            char *type_name = (char *) OPENCSTL_NIDX(&container, -4);
            CSTL_COMPARE cmp = (CSTL_COMPARE) _cmp;
            if (cmp == NULL) {
                cmp = CSTL_LESS(type_name);
            }
            if (cmp == NULL) {
                cmp = _memcmp_funcs[type_size];
            }
            if (cmp == NULL) {
                yikes("Compare function is NULL");
            }
            __cstl_list_qsort(&container, cmp);
        }
        break;
        case OPENCSTL_DEQUE: {
            size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) &container + (ptrdiff_t) (NIDX_TSIZE) * (ptrdiff_t) sizeof(size_type64) + distance);
            size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) &container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);
            char *type_name = (char *) *(_opencstl_ll_ua *) ((char *) *(void **) &container + (ptrdiff_t) (-4) * (ptrdiff_t) sizeof(size_type64) + distance);
            CSTL_COMPARE cmp = (CSTL_COMPARE) _cmp;
            if (cmp == NULL) {
                cmp = CSTL_LESS(type_name);
            }
            if (cmp == NULL) {
                cmp = _memcmp_funcs[type_size];
            }
            if (cmp == NULL) {
                yikes("Compare function is NULL");
            }
            cstl_unstable_sort(container, length, type_size, cmp);
        }
        break;
        default: {
            yikes("Invalid Operation");
        }
        break;
    }
}

// ░██████╗████████╗░█████╗░██████╗░██╗░░░░░███████╗░░░░░░░██████╗░█████╗░██████╗░████████╗
// ██╔════╝╚══██╔══╝██╔══██╗██╔══██╗██║░░░░░██╔════╝░░░░░░██╔════╝██╔══██╗██╔══██╗╚══██╔══╝
// ╚█████╗░░░░██║░░░███████║██████╦╝██║░░░░░█████╗░░░░░░░░╚█████╗░██║░░██║██████╔╝░░░██║░░░
// ░╚═══██╗░░░██║░░░██╔══██║██╔══██╗██║░░░░░██╔══╝░░░░░░░░░╚═══██╗██║░░██║██╔══██╗░░░██║░░░
// ██████╔╝░░░██║░░░██║░░██║██████╦╝███████╗███████╗█████╗██████╔╝╚█████╔╝██║░░██║░░░██║░░░
// ╚═════╝░░░░╚═╝░░░╚═╝░░╚═╝╚═════╝░╚══════╝╚══════╝╚════╝╚═════╝░░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░
#define _CSTL_STABLE_SORT_EXPAND(x) x
#define _cstl_stable_sort_func(...) _CSTL_STABLE_SORT_EXPAND(_CSTL_STABLE_SORT_DISPATCH(__VA_ARGS__, NULL, NULL))
#define _CSTL_STABLE_SORT_DISPATCH(CONTAINER, FUNC, ...) _cstl_stable_sort(CONTAINER, (void*)(FUNC))
OPENCSTL_FUNC void _cstl_stable_sort(void *container, void *_cmp) {
    ptrdiff_t distance = 0;
    size_type64 container_type = __opencstl_container_type((void **) &container, &distance);
    if (container_type == 0) return;

    switch (container_type) {
        case OPENCSTL_VECTOR: {
            size_type64 type_size = (size_type64) OPENCSTL_NIDX(&container, NIDX_TSIZE);
            char *type_name = (char *) OPENCSTL_NIDX(&container, -4);
            size_type64 length = (size_type64) OPENCSTL_NIDX(&container, -1);
            CSTL_COMPARE cmp = (CSTL_COMPARE) _cmp;
            if (cmp == NULL) {
                cmp = CSTL_LESS(type_name);
            }
            if (cmp == NULL) {
                cmp = _memcmp_funcs[type_size];
            }
            if (cmp == NULL) {
                yikes("Compare function is NULL");
            }
            cstl_best_stable_sort(container, length, type_size, cmp);
        }
        break;
        case OPENCSTL_LIST: {
            size_type64 type_size = (size_type64) OPENCSTL_NIDX(&container, NIDX_TSIZE);
            char *type_name = (char *) OPENCSTL_NIDX(&container, -4);
            CSTL_COMPARE cmp = (CSTL_COMPARE) _cmp;
            if (cmp == NULL) {
                cmp = CSTL_LESS(type_name);
            }
            if (cmp == NULL) {
                cmp = _memcmp_funcs[type_size];
            }
            if (cmp == NULL) {
                yikes("Compare function is NULL");
            }
            __cstl_list_msort(&container, cmp);
        }
        break;
        case OPENCSTL_DEQUE: {
            size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) &container + (ptrdiff_t) (NIDX_TSIZE) * (ptrdiff_t) sizeof(size_type64) + distance);
            size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) &container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);
            char *type_name = (char *) *(_opencstl_ll_ua *) ((char *) *(void **) &container + (ptrdiff_t) (-4) * (ptrdiff_t) sizeof(size_type64) + distance);
            CSTL_COMPARE cmp = (CSTL_COMPARE) _cmp;
            if (cmp == NULL) {
                cmp = CSTL_LESS(type_name);
            }
            if (cmp == NULL) {
                cmp = _memcmp_funcs[type_size];
            }
            if (cmp == NULL) {
                yikes("Compare function is NULL");
            }
            cstl_best_stable_sort(container, length, type_size, cmp);
        }
        break;
        default: {
            yikes("Invalid Operation");
        }
        break;
    }
}

#if defined(USE_CSTL_FUNC)
#if defined(__cplusplus)
#define csort _cstl_sort_func
#define cstable_sort _cstl_stable_sort_func
#else
#define sort _cstl_sort_func
#define stable_sort _cstl_stable_sort_func
#endif
#endif

#if defined(_WIN32) || defined(_WIN64)
#define cstl_sort(container,...)	_cstl_sort(&(container),ARGN(__VA_ARGS__),__VA_ARGS__)
#define cstl_stable_sort_def(container,...)	_cstl_stable_sort(&(container),ARGN(__VA_ARGS__),__VA_ARGS__)
#elif defined(__linux__) || defined(__APPLE__)

// TCC supports typeof but not __auto_type; GCC/Clang support both.
#if defined(__TINYC__)
#define _CSTL_TYPEOF(x) typeof(x)
#else
#define _CSTL_TYPEOF(x) __auto_type
#endif

#define cstl_sort(C,...) _linux_cstl_sort(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)
#define _linux_cstl_sort(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_sort ## _ ## N
#define _cstl_sort_0(C,argc)    {_CSTL_TYPEOF(&C) __0=&C;_cstl_sort( __0,argc);}
#define _cstl_sort_1(C,argc,_1)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_cstl_sort( __0,argc,&__1);}
#define _cstl_sort_2(C,argc,_1,_2)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_cstl_sort( __0,argc,&__1,&__2);}
#define _cstl_sort_3(C,argc,_1,_2,_3)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_cstl_sort( __0,argc,&__1,&__2,&__3);}
#define _cstl_sort_4(C,argc,_1,_2,_3,_4)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_cstl_sort( __0,argc,&__1,&__2,&__3,&__4);}
#define _cstl_sort_5(C,argc,_1,_2,_3,_4,_5)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_CSTL_TYPEOF(_5) __5=_5;_cstl_sort( __0,argc,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_sort_6(C,argc,_1,_2,_3,_4,_5,_6)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_CSTL_TYPEOF(_5) __5=_5;_CSTL_TYPEOF(_6) __6=_6;_cstl_sort( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_sort_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_CSTL_TYPEOF(_5) __5=_5;_CSTL_TYPEOF(_6) __6=_6;_CSTL_TYPEOF(_7) __7=_7;_cstl_sort( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_sort_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_CSTL_TYPEOF(_5) __5=_5;_CSTL_TYPEOF(_6) __6=_6;_CSTL_TYPEOF(_7) __7=_7;_CSTL_TYPEOF(_8) __8=_8;_cstl_sort( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_sort_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_CSTL_TYPEOF(_5) __5=_5;_CSTL_TYPEOF(_6) __6=_6;_CSTL_TYPEOF(_7) __7=_7;_CSTL_TYPEOF(_8) __8=_8;_CSTL_TYPEOF(_9) __9=_9;_cstl_sort( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_sort_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_CSTL_TYPEOF(_5) __5=_5;_CSTL_TYPEOF(_6) __6=_6;_CSTL_TYPEOF(_7) __7=_7;_CSTL_TYPEOF(_8) __8=_8;_CSTL_TYPEOF(_9) __9=_9;_CSTL_TYPEOF(_10) __10=_10;_cstl_sort( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}
#define cstl_stable_sort(C,...) _linux_cstl_stable_sort(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)
#define _linux_cstl_stable_sort(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_stable_sort ## _ ## N
#define _cstl_stable_sort_0(C,argc)    {_CSTL_TYPEOF(&C) __0=&C;_cstl_stable_sort( __0,argc);}
#define _cstl_stable_sort_1(C,argc,_1)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_cstl_stable_sort( __0,argc,&__1);}
#define _cstl_stable_sort_2(C,argc,_1,_2)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_cstl_stable_sort( __0,argc,&__1,&__2);}
#define _cstl_stable_sort_3(C,argc,_1,_2,_3)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_cstl_stable_sort( __0,argc,&__1,&__2,&__3);}
#define _cstl_stable_sort_4(C,argc,_1,_2,_3,_4)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_cstl_stable_sort( __0,argc,&__1,&__2,&__3,&__4);}
#define _cstl_stable_sort_5(C,argc,_1,_2,_3,_4,_5)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_CSTL_TYPEOF(_5) __5=_5;_cstl_stable_sort( __0,argc,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_stable_sort_6(C,argc,_1,_2,_3,_4,_5,_6)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_CSTL_TYPEOF(_5) __5=_5;_CSTL_TYPEOF(_6) __6=_6;_cstl_stable_sort( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_stable_sort_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_CSTL_TYPEOF(_5) __5=_5;_CSTL_TYPEOF(_6) __6=_6;_CSTL_TYPEOF(_7) __7=_7;_cstl_stable_sort( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_stable_sort_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_CSTL_TYPEOF(_5) __5=_5;_CSTL_TYPEOF(_6) __6=_6;_CSTL_TYPEOF(_7) __7=_7;_CSTL_TYPEOF(_8) __8=_8;_cstl_stable_sort( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_stable_sort_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_CSTL_TYPEOF(_5) __5=_5;_CSTL_TYPEOF(_6) __6=_6;_CSTL_TYPEOF(_7) __7=_7;_CSTL_TYPEOF(_8) __8=_8;_CSTL_TYPEOF(_9) __9=_9;_cstl_stable_sort( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_stable_sort_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(_1) __1=_1;_CSTL_TYPEOF(_2) __2=_2;_CSTL_TYPEOF(_3) __3=_3;_CSTL_TYPEOF(_4) __4=_4;_CSTL_TYPEOF(_5) __5=_5;_CSTL_TYPEOF(_6) __6=_6;_CSTL_TYPEOF(_7) __7=_7;_CSTL_TYPEOF(_8) __8=_8;_CSTL_TYPEOF(_9) __9=_9;_CSTL_TYPEOF(_10) __10=_10;_cstl_stable_sort( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}

#endif

// ██╗░██████╗░░░░░░░██████╗░█████╗░██████╗░████████╗███████╗██████╗░
// ██║██╔════╝░░░░░░██╔════╝██╔══██╗██╔══██╗╚══██╔══╝██╔════╝██╔══██╗
// ██║╚█████╗░░░░░░░╚█████╗░██║░░██║██████╔╝░░░██║░░░█████╗░░██║░░██║
// ██║░╚═══██╗░░░░░░░╚═══██╗██║░░██║██╔══██╗░░░██║░░░██╔══╝░░██║░░██║
// ██║██████╔╝█████╗██████╔╝╚█████╔╝██║░░██║░░░██║░░░███████╗██████╔╝
// ╚═╝╚═════╝░╚════╝╚═════╝░░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░╚══════╝╚═════╝░
#include "iterator.h"
#define _CSTL_IS_SORTED_EXPAND(x) x
#define _cstl_is_sorted_func(...) _CSTL_IS_SORTED_EXPAND(_CSTL_IS_SORTED_DISPATCH(__VA_ARGS__, NULL, NULL))
#define _CSTL_IS_SORTED_DISPATCH(CONTAINER, FUNC, ...) _cstl_is_sorted(CONTAINER, (void*)(FUNC))
OPENCSTL_FUNC int _cstl_is_sorted(void *container, void *_cmp) {
    ptrdiff_t distance = 0;
    size_type64 container_type = __opencstl_container_type((void **) &container, &distance);
    if (container_type == 0) return 0;

    switch (container_type) {
        case OPENCSTL_VECTOR: {
            size_type64 type_size = (size_type64) OPENCSTL_NIDX(&container, NIDX_TSIZE);
            char *type_name = (char *) OPENCSTL_NIDX(&container, -4);
            size_type64 length = (size_type64) OPENCSTL_NIDX(&container, -1);
            CSTL_COMPARE cmp = (CSTL_COMPARE) _cmp;
            if (cmp == NULL) {
                cmp = CSTL_LESS(type_name);
            }
            if (cmp == NULL) {
                cmp = _memcmp_funcs[type_size];
            }
            if (cmp == NULL) {
                yikes("Compare function is NULL");
                return 0;
            }
            for (size_type64 i = 1; i < length; i++) {
                // cmp(a, b) > 0 means a > b, so prev > curr means not sorted
                if (cmp((char *) container + (i - 1) * type_size,
                        (char *) container + i * type_size) > 0) {
                    return 0;
                }
            }
            return 1;
        }
        break;
        case OPENCSTL_LIST: {
            size_type64 type_size = (size_type64) OPENCSTL_NIDX(&container, NIDX_TSIZE);
            char *type_name = (char *) OPENCSTL_NIDX(&container, -4);
            CSTL_COMPARE cmp = (CSTL_COMPARE) _cmp;
            if (cmp == NULL) {
                cmp = CSTL_LESS(type_name);
            }
            if (cmp == NULL) {
                cmp = _memcmp_funcs[type_size];
            }
            if (cmp == NULL) {
                yikes("Compare function is NULL");
                return 0;
            }
            void *it = cstl_begin(container);
            void *end = cstl_end(container);
            if (it == end) {
                return 1;
            }
            void *prev = it;
            it = cstl_next(it);
            while (it != end) {
                if (cmp(prev, it) > 0) {
                    return 0;
                }
                prev = it;
                it = cstl_next(it);
            }
            return 1;
        }
        break;
        case OPENCSTL_DEQUE: {
            size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) &container + (ptrdiff_t) (NIDX_TSIZE) * (ptrdiff_t) sizeof(size_type64) + distance);
            size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) &container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);
            char *type_name = (char *) *(_opencstl_ll_ua *) ((char *) *(void **) &container + (ptrdiff_t) (-4) * (ptrdiff_t) sizeof(size_type64) + distance);
            CSTL_COMPARE cmp = (CSTL_COMPARE) _cmp;
            if (cmp == NULL) {
                cmp = CSTL_LESS(type_name);
            }
            if (cmp == NULL) {
                cmp = _memcmp_funcs[type_size];
            }
            if (cmp == NULL) {
                yikes("Compare function is NULL");
                return 0;
                return 0;
                return 0;
            }
            for (size_type64 i = 1; i < length; i++) {
                if (cmp((char *) container + (i - 1) * type_size,
                        (char *) container + i * type_size) > 0) {
                    return 0;
                }
            }
            return 1;
        }
        break;
        default: {
            yikes("Invalid Operation");
            return 0;
        }
        break;
    }
}

#if defined(USE_CSTL_FUNC)
#define is_sorted _cstl_is_sorted_func
#endif
#endif
