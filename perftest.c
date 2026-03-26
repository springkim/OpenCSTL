//
// Created by spring on 3/24/2026.
//
#include"opencstl/opencstl.h"
#include<stdio.h>
#include<stdlib.h>
#include <locale.h>
#if defined(__MINGW32__) || defined(__MINGW64__) || defined(__GNUC__)
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

typedef struct timeval ticktock;

static ticktock timetime() {
    ticktock tv;
    gettimeofday(&tv, NULL);
    return tv;
}

static double elapsed_time(const ticktock t_beg, const ticktock t_end) {
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

typedef LARGE_INTEGER ticktock;

static ticktock timetime() {
    ticktock t;
    QueryPerformanceCounter(&t);
    return t;
}

static double elapsed_time(const ticktock t_beg, const ticktock t_end) {
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


int main() {
    setlocale(LC_ALL, "");
    FILE *fp = NULL;
#if defined(_WIN32) || defined(_WIN64) ||defined(__TINYC__)
    fp = fopen("C:/Users/spring/Documents/GitHub/OpenCSTL/words.txt", "rt");
#elif defined(__linux__) && defined(__GNUC__)
    fp = fopen("/mnt/c/Users/spring/Documents/GitHub/OpenCSTL/words.txt", "rt");
#elif defined(__APPLE__)
    fp = fopen("/Users/spring/Documents/GitHub/OpenCSTL/words.txt", "rt");
#else
    //fopen_s(&fp, "C:/Users/spring/Documents/GitHub/OpenCSTL/all_queries.txt", "rt");
    fopen_s(&fp, "C:/Users/spring/Documents/GitHub/OpenCSTL/words1.txt", "rt");
#endif
    if (!fp) {
        printf("Error opening file\n");
    }
    char **arr = cstl_vector(char*);
    //int lines = 26410825;
    int lines = 370106;
    const int LINE_SIZE = 256;
    char *strings = (char *) calloc(lines, sizeof(char) * LINE_SIZE);
    memset(strings, 0, sizeof(char) * LINE_SIZE);
    ticktock t_beg = timetime();
    int index = 0;
    char *line = strings;
    //int max_size = 0;
    while (fgets(line, LINE_SIZE, fp)) {
        size_t len = strlen(line);

        // 마지막 문자가 개행문자인지 확인 후 제거
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // 윈도우 스타일(\r\n) 대응이 필요한 경우
        if (len > 1 && line[len - 2] == '\r') {
            line[len - 2] = '\0';
        }
        cstl_push_back(arr, line);
        //puts(line);
        line += LINE_SIZE;
        index++;
        // if (index == 500) {
        //     break;
        // }
    }
    //printf("max size: %lld\n", max_size);
    double push_back_ms = elapsed_time(t_beg, timetime());
    printf("size: %lld\telapsed_time: %lf\n",cstl_size(arr), push_back_ms);


    t_beg = timetime();

    char ***tree = cstl_set(char*, StringCmp);
    for (int i = 0; i < cstl_size(arr); i++) {
        char *line = arr[i];
        cstl_insert(tree, line);
        //puts(line);
    }
    double insert_time = elapsed_time(t_beg, timetime());
    printf("size: %lld\telapsed_time: %lf\n",cstl_size(tree), insert_time);


    // for (char **it = cstl_begin(tree); it != cstl_end(tree); it = cstl_next(it)) {
    //     printf("%s\n",cstl_value(it, char*));
    // }
    free(strings);
    cstl_free(tree);
    cstl_free(arr);
    return 0;
}
