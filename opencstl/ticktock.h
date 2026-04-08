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

#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)

#include <windows.h>
#include <stdio.h>

typedef LARGE_INTEGER watch;

static watch tick() {
    watch t;
    QueryPerformanceCounter(&t);
    return t;
}

static double tock(const watch t_beg) {
    const watch t_end = tick();
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return (double) (t_end.QuadPart - t_beg.QuadPart) * 1000.0 / (double) freq.QuadPart;
}

static void yyyy_mm_dd_hh_mm_ss_ms(char *timestr) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    snprintf(timestr, 32, "%04d_%02d_%02d_%02d_%02d_%02d_%03d",
             (int) st.wYear, (int) st.wMonth, (int) st.wDay,
             (int) st.wHour, (int) st.wMinute, (int) st.wSecond,
             (int) st.wMilliseconds);
}

#elif defined(__MINGW32__) || defined(__MINGW64__) || defined(__GNUC__) || defined(__TINYC__)

#include <stdio.h>
#include <sys/time.h>
#include <time.h>

typedef struct timeval watch;

static watch tick() {
    watch tv;
    gettimeofday(&tv, NULL);
    return tv;
}

static double tock(const watch t_beg) {
    const watch t_end = tick();
    return (t_end.tv_sec - t_beg.tv_sec) * 1000.0 +
           (t_end.tv_usec - t_beg.tv_usec) / 1000.0;
}

static void yyyy_mm_dd_hh_mm_ss_ms(char *timestr) {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    time_t now = tv.tv_sec;
    struct tm tm_now;
#if defined(__MINGW32__) || defined(__MINGW64__)
localtime_s(&tm_now, &now);
#else
localtime_r(&now, &tm_now);
#endif

int ms = (int) (tv.tv_usec / 1000);
snprintf(timestr, 32, "%04d_%02d_%02d_%02d_%02d_%02d_%03d",
         tm_now.tm_year+ 1900, tm_now.tm_mon+ 1, tm_now.tm_mday,
         tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec,
         ms);
}

#else
#error Unsupported compiler/platform
#endif

#endif
