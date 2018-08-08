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
//               Copyright (C) 2018, Woo Cheol, all rights reserved.
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
#if !defined(_OPENCSTL_STACK_H)
#define _OPENCSTL_STACK_H
#include"types.h"
#include"defines.h"
#include"error.h"

#define cstl_stack(TYPE)	__cstl_stack(sizeof(TYPE),#TYPE)
OPENCSTL_FUNC void* __cstl_stack(size_t type_size, char* type) {
	size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
	void* ptr = (char*)malloc(header_sz + type_size * 2) + header_sz; // 2 = capacity
	void** container = &ptr;
	OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_STACK;
	OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
	OPENCSTL_NIDX(container, NIDX_TSIZE) = type_size;
	OPENCSTL_NIDX(container, -4) = (size_t)type;
	OPENCSTL_NIDX(container, -3) = 2;	//capacity
	OPENCSTL_NIDX(container, -2) = 0;	//length
	*container = (char*)ptr + type_size;
	OPENCSTL_NIDX(container, -1) = -type_size - 1;	//distance  = -type_size -1 항상 음수여야 하기 때문
	return ptr;
}
#endif