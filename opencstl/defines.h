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
//               Copyright (C) 2018, Kim Bomm, all rights reserved.
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
#if !defined(_OPENCSTL_DEFINES_H)
#define _OPENCSTL_DEFINES_H

#if defined(_WIN32) || defined(_WIN64)
#define OPENCSTL_OS_WINDOWS
#elif defined(__linux__)
#define OPENCSTL_OS_LINUX
#elif defined(__APPLE__)
#define OPENCSTL_OS_OSX
#endif

#if defined(_MSC_VER)
#define OPENCSTL_CC_MSVC
#elif defined(__GNUC__)
#define OPENCSTL_CC_GCC
#elif defined(__clang__)
#define OPENCSTL_CC_CLANG
#endif


#define OPENCSTL_SET		0
#define OPENCSTL_MAP			1
#define OPENCSTL_VECTOR		2
#define OPENCSTL_LIST		3
#define OPENCSTL_DEQUE		4
#define OPENCSTL_STACK		5
#define OPENCSTL_QUEUE		6
#if defined(OPENCSTL_OS_WINDOWS) && (defined(OPENCSTL_CC_MSVC) || defined(OPENCSTL_CC_GCC))
#include<Windows.h>
#endif

//For access header element
//OPENCSTL_AccessContainerAsIndex
#define OPENCSTL_NIDX(container,nidx) (((size_t*)*container)[(nidx)])
#define OPENCSTL_HEADER	(8)
#define NIDX_CTYPE	(-8)	//container type
#define NIDX_HSIZE	(-7)	//header size
#define NIDX_TSIZE	(-6)	//type size


#ifdef _MSC_VER // Microsoft compilers
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

#define cstl_value(iter,TYPE)	(*(TYPE*)(iter+1))

#if defined(_WIN32) || defined(_WIN64)
#define __cstl_va_start(V,C,beg)	va_start(V,C);beg=V;
#define __cstl_va_arg(PTR)	(PTR)
#elif defined(__linux__)
#define __cstl_va_start(V,C,beg)	va_start(V,C);beg=((char*)V->reg_save_area)+sizeof(void*)*2;
#define __cstl_va_arg(PTR)	(*(void**)(PTR))
#elif defined(__APPLE__)
#error Not implemented in OSX
#endif

//Unary Functions
#define cstl_pop(container)	_cstl_pop(&(container))
#define cstl_pop_back(container)	_cstl_pop_back(&(container))
#define cstl_pop_front(container)	_cstl_pop_front(&(container))
#define cstl_size(container)	_cstl_size(&(container))
#define cstl_next(iterator)	_cstl_next(iterator)
#define cstl_prev(iterator)	_cstl_prev(iterator)
#define cstl_begin(container)	_cstl_begin(&(container))
#define cstl_rbegin(container)	_cstl_rbegin(&(container))
#define cstl_end(container)	_cstl_end(&(container))
#define cstl_rend(container)	_cstl_rend(&(container))
#define cstl_clear(container)	_cstl_clear(&(container))
#define cstl_free(container)	_cstl_free(&(container))
//Macro only functions

#define _cstl_deque_type(container) (*(size_t*)((char*)*(void**)container + NIDX_CTYPE * sizeof(size_t) + (OPENCSTL_NIDX(((void**)container), -1) + 1)))
#ifdef _MSC_VER
#pragma warning(disable:4047)
#pragma warning(disable:4477)
#pragma warning(disable:4313)
#define cstl_front(C)	*(is_deque((void**)&C)?\
_cstl_deque_type(&C)==OPENCSTL_DEQUE?(C):(_cstl_deque_type(&C)==OPENCSTL_QUEUE?(C):(cstl_error("Invalid Operation"))) :\
(OPENCSTL_NIDX(((void**)&C), NIDX_CTYPE)==OPENCSTL_VECTOR?(C):\
(OPENCSTL_NIDX(((void**)&C), NIDX_CTYPE)==OPENCSTL_LIST)?(C[0]):(cstl_error("Invalid Operation"))))

#define cstl_back(C)	*(is_deque((void**)&C)?\
_cstl_deque_type(&C)==OPENCSTL_DEQUE?(C+cstl_size(C)-1):(_cstl_deque_type(&C)==OPENCSTL_QUEUE?(C+cstl_size(C)-1):(cstl_error("Invalid Operation"))) :\
(OPENCSTL_NIDX(((void**)&C), NIDX_CTYPE)==OPENCSTL_VECTOR?(C+cstl_size(C)-1):\
(OPENCSTL_NIDX(((void**)&C), NIDX_CTYPE)==OPENCSTL_LIST)?(C[-2]):(cstl_error("Invalid Operation"))))
#elif	defined(__GNUC__)
#define cstl_front(C)	(is_deque(&C)?\
_cstl_deque_type(&C)==OPENCSTL_DEQUE?(C[0]):(_cstl_deque_type(&C)==OPENCSTL_QUEUE?(C[0]):(cstl_error("Invalid Operation"))) :\
(OPENCSTL_NIDX(((void**)&C), NIDX_CTYPE)==OPENCSTL_VECTOR?(C[0]):\
(OPENCSTL_NIDX(((void**)&C), NIDX_CTYPE)==OPENCSTL_LIST)?(((typeof(C)*)(C[0]))[0]):(cstl_error("Invalid Operation"))))

#define cstl_back(C)	(is_deque(&C)?\
_cstl_deque_type(&C)==OPENCSTL_DEQUE?(C[cstl_size(C)-1]):(_cstl_deque_type(&C)==OPENCSTL_QUEUE?(C[cstl_size(C)-1]):(cstl_error("Invalid Operation"))) :\
(OPENCSTL_NIDX(((void**)&C), NIDX_CTYPE)==OPENCSTL_VECTOR?(C[cstl_size(C)-1]):\
(OPENCSTL_NIDX(((void**)&C), NIDX_CTYPE)==OPENCSTL_LIST)?(((typeof(C)*)(C[-2]))[0]):(cstl_error("Invalid Operation"))))
#endif

#define OPENCSTL_DEQUE_NIDX(container, nidx) (*(size_t*)((char*)*(void**)container + nidx * sizeof(size_t) + (OPENCSTL_NIDX(((void**)container), -1) + 1)))
#define _cstl_stack_top(container)   *container[OPENCSTL_DEQUE_NIDX(container, -2) -1]
#define cstl_top(container)   is_deque(&container)?\
OPENCSTL_DEQUE_NIDX(&container, NIDX_CTYPE) == OPENCSTL_STACK ?_cstl_stack_top(&container) : (cstl_error("Invalid Operation")):\
(cstl_error("Invalid Operation"))   //priority queue


#if defined(_WIN32) || defined(_WIN64)

#define cstl_push(container,...)	_cstl_push(&(container),__VA_ARGS__)
#define cstl_push_back(container,...)	_cstl_push_back(&(container),__VA_ARGS__)
#define cstl_push_front(container,...)	_cstl_push_front(&(container),__VA_ARGS__)
#define cstl_insert(container,...)	_cstl_insert(&(container),ARGN(__VA_ARGS__),__VA_ARGS__)
#define cstl_erase(container,...)	_cstl_erase(&(container),ARGN(__VA_ARGS__),__VA_ARGS__)
#define cstl_resize(container,...)	_cstl_resize(&(container),ARGN(__VA_ARGS__),__VA_ARGS__)
#define cstl_find(container,...)	_cstl_find(&(container),ARGN(__VA_ARGS__),__VA_ARGS__)

#elif defined(__linux__)

#define cstl_push_back(C,...) _linux_cstl_push_back(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,__VA_ARGS__)
#define _linux_cstl_push_back(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_push_back ## _ ## N
#define _cstl_push_back_0(C)    {__auto_type __0=&C;_cstl_push_back( __0);}
#define _cstl_push_back_1(C,_1)    {__auto_type __0=&C;__auto_type __1=_1;_cstl_push_back( __0,&__1);}
#define _cstl_push_back_2(C,_1,_2)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;_cstl_push_back( __0,&__1,&__2);}
#define _cstl_push_back_3(C,_1,_2,_3)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;_cstl_push_back( __0,&__1,&__2,&__3);}
#define _cstl_push_back_4(C,_1,_2,_3,_4)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;_cstl_push_back( __0,&__1,&__2,&__3,&__4);}
#define _cstl_push_back_5(C,_1,_2,_3,_4,_5)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;_cstl_push_back( __0,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_push_back_6(C,_1,_2,_3,_4,_5,_6)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;_cstl_push_back( __0,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_push_back_7(C,_1,_2,_3,_4,_5,_6,_7)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;_cstl_push_back( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_push_back_8(C,_1,_2,_3,_4,_5,_6,_7,_8)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;_cstl_push_back( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_push_back_9(C,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;_cstl_push_back( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_push_back_10(C,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;__auto_type __10=_10;_cstl_push_back( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}
#define cstl_push_front(C,...) _linux_cstl_push_front(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,__VA_ARGS__)
#define _linux_cstl_push_front(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_push_front ## _ ## N
#define _cstl_push_front_0(C)    {__auto_type __0=&C;_cstl_push_front( __0);}
#define _cstl_push_front_1(C,_1)    {__auto_type __0=&C;__auto_type __1=_1;_cstl_push_front( __0,&__1);}
#define _cstl_push_front_2(C,_1,_2)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;_cstl_push_front( __0,&__1,&__2);}
#define _cstl_push_front_3(C,_1,_2,_3)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;_cstl_push_front( __0,&__1,&__2,&__3);}
#define _cstl_push_front_4(C,_1,_2,_3,_4)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;_cstl_push_front( __0,&__1,&__2,&__3,&__4);}
#define _cstl_push_front_5(C,_1,_2,_3,_4,_5)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;_cstl_push_front( __0,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_push_front_6(C,_1,_2,_3,_4,_5,_6)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;_cstl_push_front( __0,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_push_front_7(C,_1,_2,_3,_4,_5,_6,_7)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;_cstl_push_front( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_push_front_8(C,_1,_2,_3,_4,_5,_6,_7,_8)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;_cstl_push_front( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_push_front_9(C,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;_cstl_push_front( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_push_front_10(C,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;__auto_type __10=_10;_cstl_push_front( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}
#define cstl_insert(C,...) _linux_cstl_insert(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)
#define _linux_cstl_insert(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_insert ## _ ## N
#define _cstl_insert_0(C,argc)    {__auto_type __0=&C;_cstl_insert( __0,argc);}
#define _cstl_insert_1(C,argc,_1)    {__auto_type __0=&C;__auto_type __1=_1;_cstl_insert( __0,argc,&__1);}
#define _cstl_insert_2(C,argc,_1,_2)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;_cstl_insert( __0,argc,&__1,&__2);}
#define _cstl_insert_3(C,argc,_1,_2,_3)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;_cstl_insert( __0,argc,&__1,&__2,&__3);}
#define _cstl_insert_4(C,argc,_1,_2,_3,_4)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;_cstl_insert( __0,argc,&__1,&__2,&__3,&__4);}
#define _cstl_insert_5(C,argc,_1,_2,_3,_4,_5)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;_cstl_insert( __0,argc,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_insert_6(C,argc,_1,_2,_3,_4,_5,_6)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;_cstl_insert( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_insert_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;_cstl_insert( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_insert_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;_cstl_insert( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_insert_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;_cstl_insert( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_insert_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;__auto_type __10=_10;_cstl_insert( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}
#define cstl_erase(C,...) _linux_cstl_erase(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)
#define _linux_cstl_erase(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_erase ## _ ## N
#define _cstl_erase_0(C,argc)    {__auto_type __0=&C;_cstl_erase( __0,argc);}
#define _cstl_erase_1(C,argc,_1)    {__auto_type __0=&C;__auto_type __1=_1;_cstl_erase( __0,argc,&__1);}
#define _cstl_erase_2(C,argc,_1,_2)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;_cstl_erase( __0,argc,&__1,&__2);}
#define _cstl_erase_3(C,argc,_1,_2,_3)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;_cstl_erase( __0,argc,&__1,&__2,&__3);}
#define _cstl_erase_4(C,argc,_1,_2,_3,_4)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;_cstl_erase( __0,argc,&__1,&__2,&__3,&__4);}
#define _cstl_erase_5(C,argc,_1,_2,_3,_4,_5)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;_cstl_erase( __0,argc,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_erase_6(C,argc,_1,_2,_3,_4,_5,_6)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;_cstl_erase( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_erase_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;_cstl_erase( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_erase_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;_cstl_erase( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_erase_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;_cstl_erase( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_erase_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;__auto_type __10=_10;_cstl_erase( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}
#define cstl_resize(C,...) _linux_cstl_resize(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)
#define _linux_cstl_resize(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_resize ## _ ## N
#define _cstl_resize_0(C,argc)    {__auto_type __0=&C;_cstl_resize( __0,argc);}
#define _cstl_resize_1(C,argc,_1)    {__auto_type __0=&C;__auto_type __1=_1;_cstl_resize( __0,argc,&__1);}
#define _cstl_resize_2(C,argc,_1,_2)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;_cstl_resize( __0,argc,&__1,&__2);}
#define _cstl_resize_3(C,argc,_1,_2,_3)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;_cstl_resize( __0,argc,&__1,&__2,&__3);}
#define _cstl_resize_4(C,argc,_1,_2,_3,_4)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;_cstl_resize( __0,argc,&__1,&__2,&__3,&__4);}
#define _cstl_resize_5(C,argc,_1,_2,_3,_4,_5)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;_cstl_resize( __0,argc,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_resize_6(C,argc,_1,_2,_3,_4,_5,_6)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;_cstl_resize( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_resize_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;_cstl_resize( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_resize_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;_cstl_resize( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_resize_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;_cstl_resize( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_resize_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;__auto_type __10=_10;_cstl_resize( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}
#define cstl_find(C,...) ({void* _() {_linux_cstl_find(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,ARGN(__VA_ARGS__),__VA_ARGS__)}_;})();
#define _linux_cstl_find(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_find ## _ ## N
#define _cstl_find_0(C,argc)    {__auto_type __0=&C;return _cstl_find( __0,argc);}
#define _cstl_find_1(C,argc,_1)    {__auto_type __0=&C;__auto_type __1=_1;return _cstl_find( __0,argc,&__1);}
#define _cstl_find_2(C,argc,_1,_2)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;return _cstl_find( __0,argc,&__1,&__2);}
#define _cstl_find_3(C,argc,_1,_2,_3)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;return _cstl_find( __0,argc,&__1,&__2,&__3);}
#define _cstl_find_4(C,argc,_1,_2,_3,_4)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;return _cstl_find( __0,argc,&__1,&__2,&__3,&__4);}
#define _cstl_find_5(C,argc,_1,_2,_3,_4,_5)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;return _cstl_find( __0,argc,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_find_6(C,argc,_1,_2,_3,_4,_5,_6)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;return _cstl_find( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_find_7(C,argc,_1,_2,_3,_4,_5,_6,_7)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;return _cstl_find( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_find_8(C,argc,_1,_2,_3,_4,_5,_6,_7,_8)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;return _cstl_find( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_find_9(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;return _cstl_find( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_find_10(C,argc,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;__auto_type __10=_10;return _cstl_find( __0,argc,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}
#define cstl_push(C,...) _linux_cstl_push(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(C,__VA_ARGS__)
#define _linux_cstl_push(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _cstl_push ## _ ## N
#define _cstl_push_0(C)    {__auto_type __0=&C;_cstl_push( __0);}
#define _cstl_push_1(C,_1)    {__auto_type __0=&C;__auto_type __1=_1;_cstl_push( __0,&__1);}
#define _cstl_push_2(C,_1,_2)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;_cstl_push( __0,&__1,&__2);}
#define _cstl_push_3(C,_1,_2,_3)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;_cstl_push( __0,&__1,&__2,&__3);}
#define _cstl_push_4(C,_1,_2,_3,_4)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;_cstl_push( __0,&__1,&__2,&__3,&__4);}
#define _cstl_push_5(C,_1,_2,_3,_4,_5)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;_cstl_push( __0,&__1,&__2,&__3,&__4,&__5);}
#define _cstl_push_6(C,_1,_2,_3,_4,_5,_6)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;_cstl_push( __0,&__1,&__2,&__3,&__4,&__5,&__6);}
#define _cstl_push_7(C,_1,_2,_3,_4,_5,_6,_7)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;_cstl_push( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7);}
#define _cstl_push_8(C,_1,_2,_3,_4,_5,_6,_7,_8)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;_cstl_push( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8);}
#define _cstl_push_9(C,_1,_2,_3,_4,_5,_6,_7,_8,_9)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;_cstl_push( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9);}
#define _cstl_push_10(C,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10)    {__auto_type __0=&C;__auto_type __1=_1;__auto_type __2=_2;__auto_type __3=_3;__auto_type __4=_4;__auto_type __5=_5;__auto_type __6=_6;__auto_type __7=_7;__auto_type __8=_8;__auto_type __9=_9;__auto_type __10=_10;_cstl_push( __0,&__1,&__2,&__3,&__4,&__5,&__6,&__7,&__8,&__9,&__10);}

#elif defined(__APPLE__)
#error Not implemented in OSX
#endif
