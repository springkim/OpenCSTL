//
// Created by spring on 3/29/2026.
//

#if !defined(_OPENCSTL_CSTL_TIME_H)
#define _OPENCSTL_CSTL_TIME_H


#if defined(__MINGW32__) || defined(__MINGW64__) || defined(__GNUC__)
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

typedef struct timeval watch;

static watch tick() {
    watch tv;
    gettimeofday(&tv, NULL);
    return tv;
}

static double lap(const watch t_beg, const watch t_end) {
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
#elif defined(__clang__) || (defined(__GNUC__) && defined(__APPLE__)) || defined(__GNUC__)
    localtime_r(&now, &tm_now);
#else
    localtime_s(&tm_now, &now);
#endif
    int ms = (int) (tv.tv_usec / 1000);

    snprintf(timestr, 32, "%04d_%02d_%02d_%02d_%02d_%02d_%03d",
             tm_now.tm_year + 1900,
             tm_now.tm_mon + 1,
             tm_now.tm_mday,
             tm_now.tm_hour,
             tm_now.tm_min,
             tm_now.tm_sec,
             ms);
}

#elif defined(_MSC_VER) || defined(__TINYC__)  // MSVC, clang-cl

#include <windows.h>
#include <stdio.h>

typedef LARGE_INTEGER watch;

static watch tick() {
    watch t;
    QueryPerformanceCounter(&t);
    return t;
}

static double lap(const watch t_beg, const watch t_end) {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    return (double) (t_end.QuadPart - t_beg.QuadPart) * 1000.0 / (double) freq.QuadPart;
}

static void yyyy_mm_dd_hh_mm_ss_ms(char *timestr) {
    SYSTEMTIME st;
    GetLocalTime(&st);

    snprintf(timestr, 32, "%04d_%02d_%02d_%02d_%02d_%02d_%03d",
             (int) st.wYear,
             (int) st.wMonth,
             (int) st.wDay,
             (int) st.wHour,
             (int) st.wMinute,
             (int) st.wSecond,
             (int) st.wMilliseconds);
}

#else

#error Unsupported compiler/platform

#endif


#endif //_OPENCSTL_CSTL_TIME_H
