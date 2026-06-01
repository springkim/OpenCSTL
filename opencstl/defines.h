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

#if !defined(HG_E68C84B2551466CAD9CC203A4EAD4DF4B495442F0528FF84F6471E6C53DF3A88_H)
#define HG_E68C84B2551466CAD9CC203A4EAD4DF4B495442F0528FF84F6471E6C53DF3A88_H

#if defined(__cplusplus)
#define OCSTL_CPP
#endif

#if defined(_WIN32) || defined(_WIN64)
#define OPENCSTL_OS_WINDOWS
#elif defined(__linux__)
#define OPENCSTL_OS_LINUX
#elif defined(__APPLE__)
#define OPENCSTL_OS_OSX
#endif

#if defined(__clang__)
#define OPENCSTL_CC_CLANG
#elif defined(_MSC_VER)
#define OPENCSTL_CC_MSVC
#elif defined(__GNUC__)
#define OPENCSTL_CC_GCC

#endif

#if defined(_WIN32) || defined(_WIN64)
#define SELECT_ANY	__declspec(selectany)
#elif defined(__linux__) || defined(__APPLE__)
#define SELECT_ANY	__attribute__((weak))
#endif
#define OPENCSTL_FUNC	static

//#define OPENCSTL_ARRAYBASE	0x80	//b10000000
//#define OPENCSTL_NODEBASE	0x40	//b01000000

#define OPENCSTL_SET		    0
#define OPENCSTL_MAP		    1
#define OPENCSTL_VECTOR		    2
#define OPENCSTL_LIST		    3
#define OPENCSTL_DEQUE		    4
#define OPENCSTL_STACK		    5
#define OPENCSTL_QUEUE		    6
#define OPENCSTL_PRIORITY_QUEUE	7
#define OPENCSTL_UNORDERED_SET	8
#define OPENCSTL_UNORDERED_MAP	9
#define OPENCSTL_ARRAY          10


// =========

#define INTEGER32_MAX 0x7fffffff
#define OCSTL_CHAR_BIT 8


#if defined(OPENCSTL_OS_WINDOWS)
#include<Windows.h>
#endif

//For access header element
//OPENCSTL_AccessContainerAsIndex


#if defined(__GNUC__) || defined(__clang__)
typedef long long __attribute__((__aligned__(1), __may_alias__)) _opencstl_ll_ua;
#elif defined(_MSC_VER)
typedef long long __unaligned _opencstl_ll_ua;
#else
typedef long long _opencstl_ll_ua;
#endif
#define OPENCSTL_NIDX(container,nidx) (((_opencstl_ll_ua*)*container)[(nidx)])

#define OPENCSTL_HEADER	(12)
#define NIDX_CTYPE	    (-12)	// container type
#define NIDX_HSIZE	    (-11)	// header size
#define NIDX_TSIZE	    (-10)	// type size

#define NIDX_LIST_NODE_SIZE	(3)
#define NIDX_TREE_NODE_SIZE	(5)
//OPENCSTL_HEAP_MACROS
#define HEAP_PARENT(I)	    (((I)-1)>>1)
#define HEAP_LEFT(I)		(((I)<<1)+1)
#define HEAP_RIGHT(I)	    (((I)<<1)+2)

#if defined(_MSC_VER)
#   define ARGN(...)  INTERNAL_EXPAND_ARGS_PRIVATE(INTERNAL_ARGS_AUGMENTER(__VA_ARGS__))
#   define INTERNAL_ARGS_AUGMENTER(...) unused, __VA_ARGS__
#   define INTERNAL_EXPAND(x) x
#   define INTERNAL_EXPAND_ARGS_PRIVATE(...) INTERNAL_EXPAND(INTERNAL_GET_ARG_COUNT_PRIVATE(__VA_ARGS__, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#   define INTERNAL_GET_ARG_COUNT_PRIVATE(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count
#else // Non-Microsoft compilers
#   define ARGN(...) INTERNAL_GET_ARG_COUNT_PRIVATE(0, ## __VA_ARGS__, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#   define INTERNAL_GET_ARG_COUNT_PRIVATE(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count
#endif

#endif


//#define cstl_value(iter,TYPE)	(*(TYPE*)(iter+1))

#if defined(OCSTL_CC_MSVC)
// MSVC: __unaligned 키워드로 비정렬 로드 허용
#define cstl_value(iter, TYPE) \
(*(TYPE __unaligned *)((char *)(iter) + sizeof(*(iter))))
#else
// GCC / Clang / zig cc: compound literal + memcpy
// memcpy 반환값은 dst 포인터이므로 *(TYPE*) 역참조하면 정렬된 값을 읽음
#define cstl_value(iter, TYPE) \
(*(TYPE *)memcpy(&(TYPE){0}, (char *)(iter) + sizeof(*(iter)), sizeof(TYPE)))
#endif

// CSTL_PTR_STAGING: when 1, the dispatch macros stage each element into a
// typed local (via _CSTL_TYPEOF) and pass its ADDRESS through the variadic
// call, so the default argument promotions (notably float->double) never reach
// the reader. When 0, values are passed by-value and read back from the
// va_list storage -- which silently corrupts any type that gets promoted.
//
// Pointer-staging needs both a working typeof and a conforming preprocessor for
// the arg-counting / statement-expression macros further down. Linux/macOS
// (GCC/Clang/TCC) always qualify; on Windows, MinGW-GCC and Clang use the same
// GNU-conforming preprocessor and __typeof__, so they qualify too. MSVC does
// not (no typeof on older versions, traditional preprocessor), so it keeps the
// by-value path.
#if defined(OCSTL_OS_LINUX) || defined(OCSTL_OS_MACOS) || \
    (defined(OCSTL_OS_WINDOWS) && (defined(OCSTL_CC_GCC) || defined(OCSTL_CC_CLANG)))
#  define CSTL_PTR_STAGING 1
#else
#  define CSTL_PTR_STAGING 0
#endif

// CSTL_MSVC_GENERIC: 1 on MSVC built as C11 or later. MSVC keeps the by-value
// variadic path (CSTL_PTR_STAGING stays 0 above), but that path mangles a
// `float` element -- the default argument promotions turn it into a `double`
// before the reader, which copies only sizeof(float) bytes, ever sees it. From
// C11 onward _Generic lets us special-case exactly the float arguments without
// disturbing any other type (see the dispatch section below). MSVC defines
// __STDC_VERSION__ only when a /std:cNN flag selects C11+, which is precisely
// when _Generic is available, so this gate doubles as the feature check.
#if defined(OCSTL_CC_MSVC) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#  define CSTL_MSVC_GENERIC 1
#else
#  define CSTL_MSVC_GENERIC 0
#endif

// CSTL_USE_VAARG mirrors CSTL_PTR_STAGING: the staging path passes pointers, so
// the reader must use va_arg(vl, void*); the by-value path reads raw bytes.
#define CSTL_USE_VAARG CSTL_PTR_STAGING

#if CSTL_PTR_STAGING
// Pointer-staging path (Linux/macOS, Windows GCC/Clang): the dispatch macros
// pass &__N (address of a typed local copy) for each arg, so va_arg(vl, void*)
// yields the pointer to the staged value.
#define __cstl_va_start(V,C,beg)	va_start(V,C)
#define __cstl_va_arg_next(V)	    va_arg((V),void*)
#define __cstl_va_end(V)	        va_end(V)
#else
// By-value path (Windows MSVC): values are passed directly into the va_list
// storage. va_arg(vl,void*) would read the value itself, so use PTR arithmetic
// to take the address of those bytes instead.
#define __cstl_va_start(V,C,beg)	va_start(V,C);beg=(void*)V;
#define __cstl_va_arg(PTR)	(PTR)
// __cstl_va_arg_next is unused on this path, but define it to avoid compile
// errors if referenced.
#define __cstl_va_arg_next(V)	(NULL)
#define __cstl_va_end(V)	va_end(V)
#endif

//Unary Functions
#define cstl_pop(container)	        _cstl_pop(&(container))
#define cstl_pop_back(container)	_cstl_pop_back(&(container))
#define cstl_pop_front(container)	_cstl_pop_front(&(container))
#define cstl_size(container)	    _cstl_size(&(container))
#define cstl_capacity(container)	_cstl_capacity(&(container))


#define cstl_next(iterator)	        _cstl_next(iterator)
#define cstl_prev(iterator)	        _cstl_prev(iterator)
#define cstl_begin(container)	    _cstl_begin(&(container))
#define cstl_rbegin(container)	    _cstl_rbegin(&(container))
#define cstl_end(container)	        _cstl_end(&(container))
#define cstl_rend(container)	    _cstl_rend(&(container))


#define cstl_clear(container)	    _cstl_clear(&(container))
#define cstl_empty(container)	    _cstl_empty(&(container))
#define cstl_free(container)	    _cstl_free(&(container), sizeof(container))
// #define cstl_max_capacity(container) _cstl_max_size(&(container))
//Macro only functions

#define _cstl_deque_type(container) (*(_opencstl_ll_ua *)((char*)*(void**)container + (ptrdiff_t)(NIDX_CTYPE) * (ptrdiff_t)sizeof(size_type64) + (OPENCSTL_NIDX(((void**)container), -1) + 1)))
#ifdef _MSC_VER
#pragma warning(disable:4047)
#pragma warning(disable:4477)
#pragma warning(disable:4313)
#elif defined(__GNUC__) || defined(__clang__)
// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
// #pragma GCC diagnostic ignored "-Wint-conversion"
#if !defined(__clang__)
//#pragma GCC diagnostic ignored "-Wno-lto-type-mismatch"
#endif
#endif

// _cstl_deref(C, P): read the element that lives at address P, using C's
// element type so the load has the element's exact WIDTH. The old version
// always dereferenced a void* (8 bytes), which over-read any element narrower
// than a pointer -- e.g. front()/back() on VECTOR(char) returned the element
// byte plus 7 neighbouring bytes. Typing the load via __typeof__(*(C)) reads
// sizeof(element) bytes instead, so char/short (and every other scalar) come
// back correct.
//
// __typeof__ is available on GCC/Clang/TCC, and on MSVC from VS2022 17.9
// (_MSC_VER 1939) even under /std:c11. Older MSVC has no typeof, so it keeps
// the legacy pointer-width read (front()/back() stay limited there, unchanged).
#if defined(__GNUC__) || defined(__clang__) || defined(__TINYC__)
#define _cstl_deref(C, P) (__extension__ ({ \
    __typeof__(*(C)) _cstl_rv; \
    __builtin_memcpy(&_cstl_rv, (const void *)(P), sizeof(_cstl_rv)); \
    _cstl_rv; \
}))
#elif defined(_MSC_VER) && _MSC_VER >= 1939
// __unaligned permits a possibly-misaligned load (matches cstl_value above).
#define _cstl_deref(C, P) (*(__typeof__(*(C)) __unaligned *)(P))
#else
#define _cstl_deref(C, P) (*(void**)(P))
#endif
#define _cstl_err_ptr (void*)(size_type64)0

#define cstl_front(C)	_cstl_deref((C),(__is_deque((void**)&C)?\
_cstl_deque_type(&C)==OPENCSTL_DEQUE?(void*)(C):(_cstl_deque_type(&C)==OPENCSTL_QUEUE?(void*)(C):_cstl_err_ptr) :\
(OPENCSTL_NIDX(((void**)&C), NIDX_CTYPE)==OPENCSTL_VECTOR?(void*)(C):\
(OPENCSTL_NIDX(((void**)&C), NIDX_CTYPE)==OPENCSTL_LIST)?(void*)(*(void**)C):_cstl_err_ptr)))

#define cstl_back(C)	_cstl_deref((C),(__is_deque((void**)&C)?\
_cstl_deque_type(&C)==OPENCSTL_DEQUE?(void*)(C+cstl_size(C)-1):(_cstl_deque_type(&C)==OPENCSTL_QUEUE?(void*)(C+cstl_size(C)-1):_cstl_err_ptr) :\
(OPENCSTL_NIDX(((void**)&C), NIDX_CTYPE)==OPENCSTL_VECTOR?(void*)(C+cstl_size(C)-1):\
(OPENCSTL_NIDX(((void**)&C), NIDX_CTYPE)==OPENCSTL_LIST)?(void*)((void**)C)[-2]:_cstl_err_ptr)))

#if defined(__GNUC__) || defined(__clang__)
// #pragma GCC diagnostic pop
#endif

#define OPENCSTL_DEQUE_NIDX(container, nidx) (*(_opencstl_ll_ua *)((char*)*(void**)container + (ptrdiff_t)(nidx) * (ptrdiff_t)sizeof(size_type64) + (OPENCSTL_NIDX(((void**)container), -1) + 1)))
#define _cstl_stack_top(container)   *container[OPENCSTL_DEQUE_NIDX(container, -2) -1]
// cstl_top: (void**)&container explicit cast for strict compilers (MinGW64, Windows Clang).
#define cstl_top(container)   __is_deque((void**)&container)?\
OPENCSTL_DEQUE_NIDX(&container, NIDX_CTYPE) == OPENCSTL_STACK ?_cstl_stack_top(&container) : (container[0]):\
(OPENCSTL_NIDX(((void**)&container), NIDX_CTYPE)==OPENCSTL_PRIORITY_QUEUE?(*container):(container[0]))   //priority queue


#define cstl_reserve(container,n)	_cstl_reserve(&(container),n)


// _CSTL_TYPEOF: portable typeof. Used by push_back / push_front / insert / ...
// macros to declare a local of the element's type and forward its address.
// Defined here (before the platform branch) so it is available on every OS.
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
// C23 standard typeof
#define _CSTL_TYPEOF(x)  typeof(x)
#elif defined(__cplusplus)
// C++: decltype
#define _CSTL_TYPEOF(x)  decltype(x)
#elif defined(__GNUC__) || defined(__clang__) || defined(__TINYC__)
// GCC / Clang / TCC / MinGW: __typeof__
#define _CSTL_TYPEOF(x)  __typeof__(x)
#else
// MSVC and other compilers without typeof: fallback to void*.
// This works for any pointer-sized element; for larger element types the
// USE_CSTL_FUNC push_back macro is unsafe under MSVC (use cstl_push_back).
#define _CSTL_TYPEOF(x)  void*
#endif


// ---------------------------------------------------------------------------
// MSVC (C11+) float-promotion fix via _Generic.
//
// _CSTL_STAGE(x) special-cases exactly the float arguments on MSVC's by-value
// variadic path and leaves every other type untouched. A float is routed
// through _cstl_carry_float() (defined in opencstl.h), which returns the
// float's raw 4-byte pattern as an `unsigned` -- an integer-rank value that is
// never promoted -- so the va_list slot holds the bytes the reader copies back.
// Everything else (int, double, pointers, and struct rvalues such as
// make_edge(...)) falls through `default:` unchanged. The float association
// calls a *variadic* helper so the association stays constraint-valid for every
// argument type, even structs that never actually select it -- MSVC otherwise
// type-checks all _Generic associations, not just the selected one.
#if CSTL_MSVC_GENERIC

#define _CSTL_STAGE(x) _Generic((x), float: _cstl_carry_float(0, (x)), default: (x))

// _CSTL_FOREACH(m, ...) -> m(a1), m(a2), ... -- a comma map written to survive
// MSVC's traditional preprocessor (the _CSTL_EXPAND rescans force re-expansion
// of __VA_ARGS__ at each level).
#define _CSTL_EXPAND(x) x
#define _CSTL_FE_1(m, a)       m(a)
#define _CSTL_FE_2(m, a, ...)  m(a), _CSTL_EXPAND(_CSTL_FE_1(m, __VA_ARGS__))
#define _CSTL_FE_3(m, a, ...)  m(a), _CSTL_EXPAND(_CSTL_FE_2(m, __VA_ARGS__))
#define _CSTL_FE_4(m, a, ...)  m(a), _CSTL_EXPAND(_CSTL_FE_3(m, __VA_ARGS__))
#define _CSTL_FE_5(m, a, ...)  m(a), _CSTL_EXPAND(_CSTL_FE_4(m, __VA_ARGS__))
#define _CSTL_FE_6(m, a, ...)  m(a), _CSTL_EXPAND(_CSTL_FE_5(m, __VA_ARGS__))
#define _CSTL_FE_7(m, a, ...)  m(a), _CSTL_EXPAND(_CSTL_FE_6(m, __VA_ARGS__))
#define _CSTL_FE_8(m, a, ...)  m(a), _CSTL_EXPAND(_CSTL_FE_7(m, __VA_ARGS__))
#define _CSTL_FE_9(m, a, ...)  m(a), _CSTL_EXPAND(_CSTL_FE_8(m, __VA_ARGS__))
#define _CSTL_FE_10(m, a, ...) m(a), _CSTL_EXPAND(_CSTL_FE_9(m, __VA_ARGS__))
#define _CSTL_FE_PICK(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10, NAME, ...) NAME
#define _CSTL_FOREACH(m, ...) \
    _CSTL_EXPAND(_CSTL_FE_PICK(__VA_ARGS__, _CSTL_FE_10, _CSTL_FE_9, _CSTL_FE_8, \
        _CSTL_FE_7, _CSTL_FE_6, _CSTL_FE_5, _CSTL_FE_4, _CSTL_FE_3, _CSTL_FE_2, \
        _CSTL_FE_1)(m, __VA_ARGS__))

// _CSTL_VARGS(...): stage every forwarded element value.
#define _CSTL_VARGS(...) _CSTL_FOREACH(_CSTL_STAGE, __VA_ARGS__)

#else
// Pre-C11 MSVC (no _Generic): identity, preserving the original by-value path.
#define _CSTL_VARGS(...) __VA_ARGS__
#endif

// By-value dispatch path: Windows MSVC. GCC/Clang on Windows use the
// pointer-staging path below (same as Linux/macOS) so float/short/char elements
// are not corrupted by default argument promotion. On MSVC each element value
// is wrapped in _CSTL_VARGS(): under C11+ that fixes float arguments via
// _Generic; before C11 it is the identity and behavior is unchanged.
#if defined(OCSTL_OS_WINDOWS) && (defined(OCSTL_CC_MSVC) || defined(OCSTL_CC_POCC))

#define cstl_push(container,...)	_cstl_push(&(container),_CSTL_VARGS(__VA_ARGS__))
#define cstl_push_back(container,...)	_cstl_push_back(&(container),_CSTL_VARGS(__VA_ARGS__))
#define cstl_push_front(container,...)	_cstl_push_front(&(container),_CSTL_VARGS(__VA_ARGS__))
#define cstl_insert(container,...)	_cstl_insert(&(container),ARGN(__VA_ARGS__),_CSTL_VARGS(__VA_ARGS__))
#define cstl_erase(container,...)	_cstl_erase(&(container),ARGN(__VA_ARGS__),_CSTL_VARGS(__VA_ARGS__))
#define cstl_resize(container,...)	_cstl_resize(&(container),ARGN(__VA_ARGS__),_CSTL_VARGS(__VA_ARGS__))
#define cstl_assign(container,...)	_cstl_assign(&(container),ARGN(__VA_ARGS__),_CSTL_VARGS(__VA_ARGS__))
#define cstl_find(container,...)	_cstl_find(&(container),ARGN(__VA_ARGS__),_CSTL_VARGS(__VA_ARGS__))
#define cstl_shrink_to_fit(container) _cstl_shrink_to_fit(&(container))
#define cstl_max_size(container) _cstl_max_size(&container)

#define cstl_reverse(container) _cstl_reverse(&(container))


//#define cstl_count(container,...)	_cstl_count(&(container),__VA_ARGS__)

#define cstl_count_if(container,...)	_cstl_count_if(&(container),__VA_ARGS__)
#define cstl_lower_bound(container,...)	_cstl_lower_bound(&(container),ARGN(__VA_ARGS__),_CSTL_VARGS(__VA_ARGS__))
#define cstl_upper_bound(container,...)	_cstl_upper_bound(&(container),ARGN(__VA_ARGS__),_CSTL_VARGS(__VA_ARGS__))

// Pointer-staging dispatch path: Linux/macOS, plus Windows GCC/Clang.
#elif defined(OCSTL_OS_LINUX) || defined(OCSTL_OS_MACOS) || \
    (defined(OCSTL_OS_WINDOWS) && (defined(OCSTL_CC_GCC) || defined(OCSTL_CC_CLANG)))

// _CSTL_TYPEOF is now defined unconditionally above the platform branch.

#define cstl_push_back(C,...) _linux_cstl_push_back(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,__VA_ARGS__)
#define _linux_cstl_push_back(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_push_back ## _ ## N
#define _cstl_push_back_0(C)    {_CSTL_TYPEOF(&C) __0=&C;_cstl_push_back( __0);}
#define _cstl_push_back_1(C,_1)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_cstl_push_back( __0,&__1);}
#define _cstl_push_back_2(C,_1,_2)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_cstl_push_back( __0,&__1,&__2);}
#define _cstl_push_back_3(C,_1,_2,_3)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_cstl_push_back( __0,&__1,&__2,&__3);}
#define _cstl_push_back_4(C,_1,_2,_3,_4)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_cstl_push_back( __0,&__1,&__2,&__3,&__4);}
#define _cstl_push_back_5(C,_1,_2,_3,_4,_5)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_cstl_push_back( __0,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_push_back_6(C,_1,_2,_3,_4,_5,_6)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_cstl_push_back( __0,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_push_back_7(C,_1,_2,_3,_4,_5,_6,_7)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_cstl_push_back( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_push_back_8(C,_1,_2,_3,_4,_5,_6,_7,_8)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_cstl_push_back( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_push_back_9(C,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_cstl_push_back( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_push_back_10(C,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_CSTL_TYPEOF(1?(_10):(_10)) __10=(_10);_cstl_push_back( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}
#define cstl_push_front(C,...) _linux_cstl_push_front(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,__VA_ARGS__)
#define _linux_cstl_push_front(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_push_front ## _ ## N
#define _cstl_push_front_0(C)    {_CSTL_TYPEOF(&C) __0=&C;_cstl_push_front( __0);}
#define _cstl_push_front_1(C,_1)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_cstl_push_front( __0,&__1);}
#define _cstl_push_front_2(C,_1,_2)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_cstl_push_front( __0,&__1,&__2);}
#define _cstl_push_front_3(C,_1,_2,_3)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_cstl_push_front( __0,&__1,&__2,&__3);}
#define _cstl_push_front_4(C,_1,_2,_3,_4)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_cstl_push_front( __0,&__1,&__2,&__3,&__4);}
#define _cstl_push_front_5(C,_1,_2,_3,_4,_5)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_cstl_push_front( __0,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_push_front_6(C,_1,_2,_3,_4,_5,_6)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_cstl_push_front( __0,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_push_front_7(C,_1,_2,_3,_4,_5,_6,_7)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_cstl_push_front( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_push_front_8(C,_1,_2,_3,_4,_5,_6,_7,_8)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_cstl_push_front( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_push_front_9(C,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_cstl_push_front( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_push_front_10(C,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_CSTL_TYPEOF(1?(_10):(_10)) __10=(_10);_cstl_push_front( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}
#define cstl_insert(C,...) _linux_cstl_insert(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)
#define _linux_cstl_insert(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_insert ## _ ## N
#define _cstl_insert_0(C,argc)    {_CSTL_TYPEOF(&C) __0=&C;_cstl_insert( __0,argc);}
#define _cstl_insert_1(C,argc,_1)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_cstl_insert( __0,argc,&__1);}
#define _cstl_insert_2(C,argc,_1,_2)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_cstl_insert( __0,argc,&__1,&__2);}
#define _cstl_insert_3(C,argc,_1,_2,_3)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_cstl_insert( __0,argc,&__1,&__2,&__3);}
#define _cstl_insert_4(C,argc,_1,_2,_3,_4)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_cstl_insert( __0,argc,&__1,&__2,&__3,&__4);}
#define _cstl_insert_5(C,argc,_1,_2,_3,_4,_5)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_cstl_insert( __0,argc,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_insert_6(C,argc,_1,_2,_3,_4,_5,_6)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_cstl_insert( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_insert_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_cstl_insert( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_insert_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_cstl_insert( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_insert_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_cstl_insert( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_insert_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_CSTL_TYPEOF(1?(_10):(_10)) __10=(_10);_cstl_insert( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}
#define cstl_erase(C,...) _linux_cstl_erase(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)
#define _linux_cstl_erase(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_erase ## _ ## N
#define _cstl_erase_0(C,argc)    {_CSTL_TYPEOF(&C) __0=&C;_cstl_erase( __0,argc);}
#define _cstl_erase_1(C,argc,_1)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_cstl_erase( __0,argc,&__1);}
#define _cstl_erase_2(C,argc,_1,_2)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_cstl_erase( __0,argc,&__1,&__2);}
#define _cstl_erase_3(C,argc,_1,_2,_3)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_cstl_erase( __0,argc,&__1,&__2,&__3);}
#define _cstl_erase_4(C,argc,_1,_2,_3,_4)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_cstl_erase( __0,argc,&__1,&__2,&__3,&__4);}
#define _cstl_erase_5(C,argc,_1,_2,_3,_4,_5)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_cstl_erase( __0,argc,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_erase_6(C,argc,_1,_2,_3,_4,_5,_6)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_cstl_erase( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_erase_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_cstl_erase( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_erase_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_cstl_erase( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_erase_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_cstl_erase( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_erase_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_CSTL_TYPEOF(1?(_10):(_10)) __10=(_10);_cstl_erase( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}
#define cstl_resize(C,...) _linux_cstl_resize(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)
#define _linux_cstl_resize(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_resize ## _ ## N
#define _cstl_resize_0(C,argc)    {_CSTL_TYPEOF(&C) __0=&C;_cstl_resize( __0,argc);}
#define _cstl_resize_1(C,argc,_1)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_cstl_resize( __0,argc,&__1);}
#define _cstl_resize_2(C,argc,_1,_2)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_cstl_resize( __0,argc,&__1,&__2);}
#define _cstl_resize_3(C,argc,_1,_2,_3)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_cstl_resize( __0,argc,&__1,&__2,&__3);}
#define _cstl_resize_4(C,argc,_1,_2,_3,_4)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_cstl_resize( __0,argc,&__1,&__2,&__3,&__4);}
#define _cstl_resize_5(C,argc,_1,_2,_3,_4,_5)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_cstl_resize( __0,argc,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_resize_6(C,argc,_1,_2,_3,_4,_5,_6)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_cstl_resize( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_resize_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_cstl_resize( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_resize_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_cstl_resize( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_resize_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_cstl_resize( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_resize_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_CSTL_TYPEOF(1?(_10):(_10)) __10=(_10);_cstl_resize( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}
#define cstl_assign(C,...) _linux_cstl_assign(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)
#define _linux_cstl_assign(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_assign ## _ ## N
#define _cstl_assign_0(C,argc)    {_CSTL_TYPEOF(&C) __0=&C;_cstl_assign( __0,argc);}
#define _cstl_assign_1(C,argc,_1)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_cstl_assign( __0,argc,&__1);}
#define _cstl_assign_2(C,argc,_1,_2)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_cstl_assign( __0,argc,&__1,&__2);}
#define _cstl_assign_3(C,argc,_1,_2,_3)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_cstl_assign( __0,argc,&__1,&__2,&__3);}
#define _cstl_assign_4(C,argc,_1,_2,_3,_4)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_cstl_assign( __0,argc,&__1,&__2,&__3,&__4);}
#define _cstl_assign_5(C,argc,_1,_2,_3,_4,_5)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_cstl_assign( __0,argc,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_assign_6(C,argc,_1,_2,_3,_4,_5,_6)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_cstl_assign( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_assign_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_cstl_assign( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_assign_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_cstl_assign( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_assign_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_cstl_assign( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_assign_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_CSTL_TYPEOF(1?(_10):(_10)) __10=(_10);_cstl_assign( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}

#define cstl_find(C,...) _linux_cstl_find(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)
#define _linux_cstl_find(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_find ## _ ## N
#define _cstl_find_0(C,argc)    ({_CSTL_TYPEOF(&C) __0=&C;_cstl_find( __0,argc);})
#define _cstl_find_1(C,argc,_1)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_cstl_find( __0,argc,&__1);}))
#define _cstl_find_2(C,argc,_1,_2)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_cstl_find( __0,argc,&__1,&__2);}))
#define _cstl_find_3(C,argc,_1,_2,_3)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_cstl_find( __0,argc,&__1,&__2,&__3);}))
#define _cstl_find_4(C,argc,_1,_2,_3,_4)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_cstl_find( __0,argc,&__1,&__2,&__3,&__4);}))
#define _cstl_find_5(C,argc,_1,_2,_3,_4,_5)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_cstl_find( __0,argc,&__1,&__2,&__3,&__4,&__5);}))
#define _cstl_find_6(C,argc,_1,_2,_3,_4,_5,_6)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_cstl_find( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}))
#define _cstl_find_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_cstl_find( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}))
#define _cstl_find_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_cstl_find( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}))
#define _cstl_find_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_cstl_find( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}))
#define _cstl_find_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_CSTL_TYPEOF(1?(_10):(_10)) __10=(_10);_cstl_find( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}))
#define cstl_push(C,...) _linux_cstl_push(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,__VA_ARGS__)
#define _linux_cstl_push(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_push ## _ ## N
#define _cstl_push_0(C)    {_CSTL_TYPEOF(&C) __0=&C;_cstl_push( __0);}
#define _cstl_push_1(C,_1)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_cstl_push( __0,&__1);}
#define _cstl_push_2(C,_1,_2)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_cstl_push( __0,&__1,&__2);}
#define _cstl_push_3(C,_1,_2,_3)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_cstl_push( __0,&__1,&__2,&__3);}
#define _cstl_push_4(C,_1,_2,_3,_4)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_cstl_push( __0,&__1,&__2,&__3,&__4);}
#define _cstl_push_5(C,_1,_2,_3,_4,_5)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_cstl_push( __0,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_push_6(C,_1,_2,_3,_4,_5,_6)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_cstl_push( __0,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_push_7(C,_1,_2,_3,_4,_5,_6,_7)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_cstl_push( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_push_8(C,_1,_2,_3,_4,_5,_6,_7,_8)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_cstl_push( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_push_9(C,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_cstl_push( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_push_10(C,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_CSTL_TYPEOF(1?(_10):(_10)) __10=(_10);_cstl_push( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}


#define cstl_max_size(container) _cstl_max_size(&container)
#define cstl_shrink_to_fit(container) _cstl_shrink_to_fit(&(container))
#define cstl_reverse(container) _cstl_reverse(&(container))


#define cstl_count(C,...) _linux_cstl_count(C,__VA_ARGS__, 5, 4, 3, 2, 1, 0)(C,__VA_ARGS__)
#define _linux_cstl_count(C,_1, _2, _3, _4, _5, N, ...) _cstl_count ## _ ## N
#define _cstl_count_0(C)    (({_CSTL_TYPEOF(&C) __0=&C;_cstl_count( __0);}))
#define _cstl_count_1(C,_1)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_cstl_count( __0,&__1);}))
#define _cstl_count_2(C,_1,_2)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_cstl_count( __0,&__1,&__2);}))
#define _cstl_count_3(C,_1,_2,_3)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_cstl_count( __0,&__1,&__2,&__3);}))
#define _cstl_count_4(C,_1,_2,_3,_4)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_cstl_count( __0,&__1,&__2,&__3,&__4);}))
#define _cstl_count_5(C,_1,_2,_3,_4,_5)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_cstl_count( __0,&__1,&__2,&__3,&__4,&__5);}))

#define cstl_count_if(C,...) _linux_cstl_count_if(C,__VA_ARGS__, 5, 4, 3, 2, 1, 0)(C,__VA_ARGS__)
#define _linux_cstl_count_if(C,_1, _2, _3, _4, _5, N, ...) _cstl_count_if ## _ ## N
#define _cstl_count_if_0(C)    (({_CSTL_TYPEOF(&C) __0=&C;_cstl_count_if( __0);}))
#define _cstl_count_if_1(C,_1)    (({_CSTL_TYPEOF(&C) __0=&C;_cstl_count_if( __0,_1);}))
#define _cstl_count_if_2(C,_1,_2)    (({_CSTL_TYPEOF(&C) __0=&C;_cstl_count_if( __0,_1,_2);}))
#define _cstl_count_if_3(C,_1,_2,_3)    (({_CSTL_TYPEOF(&C) __0=&C;_cstl_count_if( __0,_1,_2,_3);}))
#define _cstl_count_if_4(C,_1,_2,_3,_4)    (({_CSTL_TYPEOF(&C) __0=&C;_cstl_count_if( __0,_1,_2,_3,_4);}))
#define _cstl_count_if_5(C,_1,_2,_3,_4,_5)    (({_CSTL_TYPEOF(&C) __0=&C;_cstl_count_if( __0,_1,_2,_3,_4,_5);}))

#define cstl_lower_bound(C,...) _linux_cstl_lower_bound(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)
#define _linux_cstl_lower_bound(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_lower_bound ## _ ## N
#define _cstl_lower_bound_0(C,argc)    (({_CSTL_TYPEOF(&C) __0=&C;_cstl_lower_bound( __0,argc);}))
#define _cstl_lower_bound_1(C,argc,_1)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_cstl_lower_bound( __0,argc,&__1);}))
#define _cstl_lower_bound_2(C,argc,_1,_2)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_cstl_lower_bound( __0,argc,&__1,&__2);}))
#define _cstl_lower_bound_3(C,argc,_1,_2,_3)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_cstl_lower_bound( __0,argc,&__1,&__2,&__3);}))
#define _cstl_lower_bound_4(C,argc,_1,_2,_3,_4)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_cstl_lower_bound( __0,argc,&__1,&__2,&__3,&__4);}))
#define _cstl_lower_bound_5(C,argc,_1,_2,_3,_4,_5)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_cstl_lower_bound( __0,argc,&__1,&__2,&__3,&__4,&__5);}))
#define _cstl_lower_bound_6(C,argc,_1,_2,_3,_4,_5,_6)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_cstl_lower_bound( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}))
#define _cstl_lower_bound_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_cstl_lower_bound( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}))
#define _cstl_lower_bound_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_cstl_lower_bound( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}))
#define _cstl_lower_bound_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_cstl_lower_bound( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}))
#define _cstl_lower_bound_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_CSTL_TYPEOF(1?(_10):(_10)) __10=(_10);_cstl_lower_bound( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}))

#define cstl_upper_bound(C,...) _linux_cstl_upper_bound(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)
#define _linux_cstl_upper_bound(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_upper_bound ## _ ## N
#define _cstl_upper_bound_0(C,argc)    (({_CSTL_TYPEOF(&C) __0=&C;_cstl_upper_bound( __0,argc);}))
#define _cstl_upper_bound_1(C,argc,_1)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_cstl_upper_bound( __0,argc,&__1);}))
#define _cstl_upper_bound_2(C,argc,_1,_2)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_cstl_upper_bound( __0,argc,&__1,&__2);}))
#define _cstl_upper_bound_3(C,argc,_1,_2,_3)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_cstl_upper_bound( __0,argc,&__1,&__2,&__3);}))
#define _cstl_upper_bound_4(C,argc,_1,_2,_3,_4)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_cstl_upper_bound( __0,argc,&__1,&__2,&__3,&__4);}))
#define _cstl_upper_bound_5(C,argc,_1,_2,_3,_4,_5)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_cstl_upper_bound( __0,argc,&__1,&__2,&__3,&__4,&__5);}))
#define _cstl_upper_bound_6(C,argc,_1,_2,_3,_4,_5,_6)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_cstl_upper_bound( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}))
#define _cstl_upper_bound_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_cstl_upper_bound( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}))
#define _cstl_upper_bound_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_cstl_upper_bound( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}))
#define _cstl_upper_bound_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_cstl_upper_bound( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}))
#define _cstl_upper_bound_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    (({_CSTL_TYPEOF(&C) __0=&C;_CSTL_TYPEOF(1?(_1):(_1)) __1=(_1);_CSTL_TYPEOF(1?(_2):(_2)) __2=(_2);_CSTL_TYPEOF(1?(_3):(_3)) __3=(_3);_CSTL_TYPEOF(1?(_4):(_4)) __4=(_4);_CSTL_TYPEOF(1?(_5):(_5)) __5=(_5);_CSTL_TYPEOF(1?(_6):(_6)) __6=(_6);_CSTL_TYPEOF(1?(_7):(_7)) __7=(_7);_CSTL_TYPEOF(1?(_8):(_8)) __8=(_8);_CSTL_TYPEOF(1?(_9):(_9)) __9=(_9);_CSTL_TYPEOF(1?(_10):(_10)) __10=(_10);_cstl_upper_bound( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}))

#endif

// max_element / min_element take a comparator (or none) -- never a by-value
// element -- so they need no promotion staging and are identical on every
// platform/compiler. Defined here (outside the dispatch branch) so that the
// MSVC by-value path and the GCC/Clang/Linux staging path both get them.
#ifndef cstl_max_element
#define cstl_max_element(C, ...) ocstl_min_max_element(&(C), 1LL, ##__VA_ARGS__, NULL)
#endif
#ifndef cstl_min_element
#define cstl_min_element(C, ...) ocstl_min_max_element(&(C), 0LL, ##__VA_ARGS__, NULL)
#endif
