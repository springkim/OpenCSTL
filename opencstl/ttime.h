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
#if !defined(_OPENCSTL_CSTL_TICKTOCK_H)
#define _OPENCSTL_CSTL_TICKTOCK_H


#include <stdio.h>


#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)

#include <windows.h>

static double ttime(void) {
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double) counter.QuadPart * 1000.0 / (double) freq.QuadPart;
}

#elif defined(__MINGW32__) || defined(__MINGW64__) || defined(__GNUC__) || defined(__TINYC__)

#include <sys/time.h>
#include <time.h>

static double ttime(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double) tv.tv_sec * 1000.0 + (double) tv.tv_usec / 1000.0;
}

#else
#error Unsupported compiler/platform
#endif


#endif
