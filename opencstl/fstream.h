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
#if !defined(_OPENCSTL_CSTL_FILE_H)
#define _OPENCSTL_CSTL_FILE_H
#include <stdio.h>
#include "fileio.h"

FILE *__cstl_fopen(const char *filename, const char *mode) {
    FILE *fp = NULL;
#if defined(__TINYC__)
    fp = fopen(filename, mode);
#elif defined(_WIN32) || defined(_WIN64)
    fopen_s(&fp, filename, mode);
#else
    fp = fopen(filename, mode);
#endif
    return fp;
}

void __cstl_fclose(FILE *fp) {
    fclose(fp);
}

char *__cstl_get_line(FILE *fp) {
#define _MAX_LINE_SIZE 65536*2*2 // 256KB
    char line[_MAX_LINE_SIZE] = {0};
    int c = 0;
    size_t i = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') break;
        if (i + 1 < _MAX_LINE_SIZE) {
            line[i++] = (char) c;
        }
    }
    if (i == 0) {
        return NULL;
    }
    line[i] = '\0';
    char *ret = (char *) calloc(i + 1, sizeof(char));
    memcpy(ret, line, i);
    return ret;
}

wchar_t *__cstl_get_line_w(FILE *fp) {
#define _MAX_LINE_SIZE 65536*2*2 // 256KB
    wchar_t line[_MAX_LINE_SIZE] = {0};
    wint_t c = 0;
    size_t i = 0;
    while ((c = fgetwc(fp)) != EOF) {
        if (c == L'\n') break;
        if (i + 1 < _MAX_LINE_SIZE) {
            line[i++] = (wchar_t) c;
        }
    }
    if (i == 0) {
        return NULL;
    }
    line[i] = L'\0';
    wchar_t *ret = (wchar_t *) calloc(i + 1, sizeof(wchar_t));
    memcpy(ret, line, i * sizeof(wchar_t));
    return ret;
}

char *__cstl_fread_all(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    size_t total_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buf = (char *) calloc(total_size, sizeof(char));
    fread(buf, 1, total_size, fp);
    return buf;
}

typedef FILE *(*cstl_fopen_fn)(const char *filename, const char *mode);

typedef void (*cstl_fclose_fn)(FILE *fp);

typedef char *(*cstl_getline_fn)(FILE *fp);

typedef char *(*cstl_fread_all_fn)(FILE *fp);

typedef char *(*cstl_readall_fn)(FILE *fp, size_t *outsize);

typedef struct FSTREAM {
    cstl_fopen_fn open;
    cstl_fclose_fn close;
    cstl_getline_fn getline;
    cstl_fread_all_fn read;
} FSTREAM;

FSTREAM fstream = {
    __cstl_fopen,
    __cstl_fclose,
    __cstl_get_line,
    __cstl_fread_all,
   // fread_all
};


#endif //_OPENCSTL_CSTL_FILE_H
