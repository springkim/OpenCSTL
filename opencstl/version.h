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
#if !defined(_OPENCSTL_VERSION_H)
#define _OPENCSTL_VERSION_H

static char *OPENCSTL_VERSION = "1.1.0";

static char *opencstl_version() {
    return OPENCSTL_VERSION;
}

#if defined(_WIN32) || defined(_WIN64)
#define OPENCSTL_OS "Windows"
#elif defined(__linux__)
#define OPENCSTL_OS "Linux"
#elif defined(__APPLE__)
#define OPENCSTL_OS "MacOS"
#endif

#if defined(__clang__)
#define OPENCSTL_CC "Clang"
#elif defined(__TINYC__)
#define OPENCSTL_CC "TCC"
#elif defined(_MSC_VER)
#define OPENCSTL_CC "MSVC"
#elif defined(__GNUC__)
#define OPENCSTL_CC "GCC"
#endif


#if defined(__STDC_VERSION__)
#   if __STDC_VERSION__ >= 202311L
#define OPENCSTL_CV "C23"
#   elif __STDC_VERSION__ >= 201710L
#define OPENCSTL_CV "C17 (C18)"
#   elif __STDC_VERSION__ >= 201112L
#define OPENCSTL_CV "C11"
#   elif __STDC_VERSION__ >= 199901L
#define OPENCSTL_CV "C99"
#   else
#define OPENCSTL_CV "C94 (AMD1)"
#   endif
#elif defined(__STDC__)
#define OPENCSTL_CV "C89 (C90)"
#else
#define OPENCSTL_CV "pre-ANSI C (K&R)"
#endif

static char __opencstl_env_str[512] = {0};

char *opencstl_env() {
    sprintf(__opencstl_env_str, "%s, %s, %s",OPENCSTL_OS,OPENCSTL_CC,OPENCSTL_CV);
    return __opencstl_env_str;
}
#endif //_OPENCSTL_VERSION_H
