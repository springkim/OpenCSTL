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
#pragma once
#ifndef _OPENCSTL_CROSSPLATFORM_H
#define _OPENCSTL_CROSSPLATFORM_H


// ░█████╗░░██████╗
// ██╔══██╗██╔════╝
// ██║░░██║╚█████╗░
// ██║░░██║░╚═══██╗
// ╚█████╔╝██████╔╝
// ░╚════╝░╚═════╝░
#if defined(_WIN32) || defined(_WIN64)
#define OCSTL_OS_STR "Windows"
#define OCSTL_OS_WINDOWS
#elif defined(__linux__)
#define OCSTL_OS_STR "Linux"
#define OCSTL_OS_LINUX
#elif defined(__APPLE__)
#define OCSTL_OS_STR "MacOS"
#define OCSTL_OS_MACOS
#endif

// ░█████╗░░█████╗░███╗░░░███╗██████╗░██╗██╗░░░░░███████╗██████
// ██╔══██╗██╔══██╗████╗░████║██╔══██╗██║██║░░░░░██╔════╝██╔══██╗
// ██║░░╚═╝██║░░██║██╔████╔██║██████╔╝██║██║░░░░░█████╗░░██████╔╝
// ██║░░██╗██║░░██║██║╚██╔╝██║██╔═══╝░██║██║░░░░░██╔══╝░░██╔══██╗
// ╚█████╔╝╚█████╔╝██║░╚═╝░██║██║░░░░░██║███████╗███████╗██║░░██║
// ░╚════╝░░╚════╝░╚═╝░░░░░╚═╝╚═╝░░░░░╚═╝╚══════╝╚══════╝╚═╝░░╚═╝
#if defined(__POCC__)
#define OCSTL_CC_STR "PellesC"
#define OCSTL_CC_POCC
#elif defined(__NVCC__)
#define OCSTL_CC_STR "NVCC"
#define OCSTL_CC_NVCC
#elif defined(__clang__)
#define OCSTL_CC_STR "Clang"
#define OCSTL_CC_CLANG
#elif defined(__TINYC__)
#define OCSTL_CC_STR "TCC"
#define OCSTL_CC_TCC
#elif defined(_MSC_VER)
#define OCSTL_CC_STR "MSVC"
#define OCSTL_CC_MSVC
#elif defined(__GNUC__)
#define OCSTL_CC_STR "GCC"
#define OCSTL_CC_GCC
#endif


// ░█████╗░░░░░░░██╗░░░██╗███████╗██████╗░░██████╗██╗░█████╗░███╗░░██╗
// ██╔══██╗░░░░░░██║░░░██║██╔════╝██╔══██╗██╔════╝██║██╔══██╗████╗░██║
// ██║░░╚═╝█████╗╚██╗░██╔╝█████╗░░██████╔╝╚█████╗░██║██║░░██║██╔██╗██║
// ██║░░██╗╚════╝░╚████╔╝░██╔══╝░░██╔══██╗░╚═══██╗██║██║░░██║██║╚████║
// ╚█████╔╝░░░░░░░░╚██╔╝░░███████╗██║░░██║██████╔╝██║╚█████╔╝██║░╚███║
// ░╚════╝░░░░░░░░░░╚═╝░░░╚══════╝╚═╝░░╚═╝╚═════╝░╚═╝░╚════╝░╚═╝░░╚══╝

#if defined(__STDC_VERSION__)
#   if __STDC_VERSION__ >= 202311L
#define OCSTL_C_VERSION_STR "C23"
#define OCSTL_C_VERSION_23
#   elif __STDC_VERSION__ >= 201710L
#define OCSTL_C_VERSION_STR "C17 (C18)"
#define OCSTL_C_VERSION_17
#   elif __STDC_VERSION__ >= 201112L
#define OCSTL_C_VERSION_STR "C11"
#define OCSTL_C_VERSION_11
#   elif __STDC_VERSION__ >= 199901L
#define OCSTL_C_VERSION_STR "C99"
#define OCSTL_C_VERSION_99
#   else
#define OCSTL_C_VERSION_STR "C94 (AMD1)"
#define OCSTL_C_VERSION_94
#   endif
#elif defined(__STDC__)
#define OCSTL_C_VERSION_STR "C89 (C90)"
#define OCSTL_C_VERSION_89
#else
#define OCSTL_C_VERSION_STR "pre-ANSI C (K&R)"
#define OCSTL_C_VERSION_PRE_ANSI
#endif


// ░█████╗░██████╗░████████╗██╗███╗░░░███╗██╗███████╗███████╗
// ██╔══██╗██╔══██╗╚══██╔══╝██║████╗░████║██║╚════██║██╔════╝
// ██║░░██║██████╔╝░░░██║░░░██║██╔████╔██║██║░░███╔═╝█████╗░░
// ██║░░██║██╔═══╝░░░░██║░░░██║██║╚██╔╝██║██║██╔══╝░░██╔══╝░░
// ╚█████╔╝██║░░░░░░░░██║░░░██║██║░╚═╝░██║██║███████╗███████╗
// ░╚════╝░╚═╝░░░░░░░░╚═╝░░░╚═╝╚═╝░░░░░╚═╝╚═╝╚══════╝╚══════╝

#if defined(_DEBUG) || !defined(NDEBUG)
#define OCSTL_DEBUG
#else
#define OCSTL_RELEASE
#endif


#endif //_OPENCSTL_CROSSPLATFORM_H
