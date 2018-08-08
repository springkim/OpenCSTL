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
#if !defined(_OPENCSTL_ERROR_H)
#define _OPENCSTL_ERROR_H
#include"types.h"
#include"defines.h"
#define cstl_error(msg)		__cstl_error(msg,__FILE__,__FUNCTION__,__LINE__)
OPENCSTL_FUNC int __cstl_error(const char* msg, const char* file, const char* function, int line) {
	char err_msg[1024] = { 0 };
#if defined(OPENCSTL_OS_WINDOWS) && (defined(OPENCSTL_CC_MSVC) || defined(OPENCSTL_CC_GCC))
	sprintf_s(err_msg,1024, "[%s] in %s , %s , %d\n", msg, file, function, line);
	MessageBoxA(NULL, err_msg, "ccl fatal", MB_OK);
#else
	sprintf(err_msg, "[%s] in %s , %s , %d\n", msg, file, function, line);
	//Other platform msg box...
#endif
	exit(EXIT_FAILURE);
	return 0;
}
#endif