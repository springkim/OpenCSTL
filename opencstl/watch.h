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

typedef LARGE_INTEGER watch;

static watch now() {
    watch t;
    QueryPerformanceCounter(&t);
    return t;
}

static double duration(const watch t_beg, const watch t_end) {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    double ms = (double) (t_end.QuadPart - t_beg.QuadPart) * 1000.0 / (double) freq.QuadPart;
    return ms > 0 ? ms : -ms;
}


#elif defined(__MINGW32__) || defined(__MINGW64__) || defined(__GNUC__) || defined(__TINYC__)


#include <sys/time.h>
#include <time.h>

typedef struct timeval watch;

static watch now(void) {
    watch tv;
    gettimeofday(&tv, NULL);
    return tv;
}

static double duration(const watch t_beg, const watch t_end) {
    double ms = (t_end.tv_sec - t_beg.tv_sec) * 1000.0 +
                (t_end.tv_usec - t_beg.tv_usec) / 1000.0;
    return ms > 0 ? ms : -ms;
}


#else
#error Unsupported compiler/platform
#endif

#endif
