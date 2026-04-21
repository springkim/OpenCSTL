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
#include <wchar.h>

// ███████╗██╗██╗░░░░░███████╗░░░░░░░██████╗░░█████╗░██╗███╗░░██╗████████╗███████╗██████╗░░░░░░░░███╗░░░███╗░█████╗░███╗░░██╗░█████╗░░██████╗░███████╗██████╗░
// ██╔════╝██║██║░░░░░██╔════╝░░░░░░░██╔══██╗██╔══██╗██║████╗░██║╚══██╔══╝██╔════╝██╔══██╗░░░░░░░████╗░████║██╔══██╗████╗░██║██╔══██╗██╔════╝░██╔════╝██╔══██╗
// █████╗░░██║██║░░░░░█████╗░░░░░░░░░██████╔╝██║░░██║██║██╔██╗██║░░░██║░░░█████╗░░██████╔╝░░░░░░░██╔████╔██║███████║██╔██╗██║███████║██║░░██╗░█████╗░░██████╔╝
// ██╔══╝░░██║██║░░░░░██╔══╝░░░░░░░░░██╔═══╝░██║░░██║██║██║╚████║░░░██║░░░██╔══╝░░██╔══██╗░░░░░░░██║╚██╔╝██║██╔══██║██║╚████║██╔══██║██║░░╚██╗██╔══╝░░██╔══██╗
// ██║░░░░░██║███████╗███████╗░░░░░░░██║░░░░░╚█████╔╝██║██║░╚███║░░░██║░░░███████╗██║░░██║░░░░░░░██║░╚═╝░██║██║░░██║██║░╚███║██║░░██║╚██████╔╝███████╗██║░░██║
// ╚═╝░░░░░╚═╝╚══════╝╚══════╝░░░░░░░╚═╝░░░░░░╚════╝░╚═╝╚═╝░░╚══╝░░░╚═╝░░░╚══════╝╚═╝░░╚═╝░░░░░░░╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝░░╚══╝╚═╝░░╚═╝░╚═════╝░╚══════╝╚═╝░░╚═╝

typedef struct FPM {
    FILE *fp;
    char *filepath;
} FPM;

static FPM fpm[1024] = {0};
static int fpm_size = 0;

void fpm_append(FILE *fp, char *filepath) {
    verify(fpm_size < 1024);
    fpm[fpm_size].fp = fp;
    fpm[fpm_size].filepath = filepath;
    fpm_size++;
}

void fpm_erase(FILE *fp) {
    int idx = -1;
    for (int i = 0; i < fpm_size; i++) {
        if (fpm[i].fp == fp) {
            idx = i;
            break;
        }
    }
    verify(idx != -1);
    memcpy(fpm + idx, fpm + idx + 1, fpm_size - idx - 1);
    fpm_size--;
}

char *fpm_get(FILE *fp) {
    int idx = -1;
    for (int i = 0; i < fpm_size; i++) {
        if (fpm[i].fp == fp) {
            idx = i;
            break;
        }
    }
    verify(idx != -1);
    return fpm[idx].filepath;
}

// ███████╗░██████╗████████╗██████╗░███████╗░█████╗░███╗░░░███╗
// ██╔════╝██╔════╝╚══██╔══╝██╔══██╗██╔════╝██╔══██╗████╗░████║
// █████╗░░╚█████╗░░░░██║░░░██████╔╝█████╗░░███████║██╔████╔██║
// ██╔══╝░░░╚═══██╗░░░██║░░░██╔══██╗██╔══╝░░██╔══██║██║╚██╔╝██║
// ██║░░░░░██████╔╝░░░██║░░░██║░░██║███████╗██║░░██║██║░╚═╝░██║
// ╚═╝░░░░░╚═════╝░░░░╚═╝░░░╚═╝
FILE *__cstl_fopen( char *filename, const char *mode) {
    FILE *fp = NULL;
#if defined(__TINYC__)
    fp = fopen(filename, mode);
#elif defined(_WIN32) || defined(_WIN64)
    fopen_s(&fp, filename, mode);
#else
    fp = fopen(filename, mode);
#endif
    verify(fp != NULL);
    fpm_append(fp, filename);
    return fp;
}

void __cstl_fclose(FILE *fp) {

    fpm_erase(fp);
    fclose(fp);
}


char *__cstl_get_line(FILE *fp) {
    long start = ftell(fp);
    if (start < 0) return NULL;

    size_t len = 0;
    int c;
    int found_any = 0;
    while ((c = fgetc(fp)) != EOF) {
        found_any = 1;
        if (c == '\n') break;
        len++;
    }
    if (!found_any && len == 0) return NULL;

    if (fseek(fp, start, SEEK_SET) != 0) return NULL;

    char *buf = (char *) calloc(len + 1, sizeof(char));
    if (!buf) return NULL;

    for (size_t i = 0; i < len; i++) {
        int ch = fgetc(fp);
        if (ch == EOF) {
            free(buf);
            return NULL;
        }
        buf[i] = (char) ch;
    }
    (void) fgetc(fp);
    return buf;
}

char *__cstl_fread_all(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    size_t total_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buf = (char *) calloc(total_size, sizeof(char));
    fread(buf, 1, total_size, fp);
    return buf;
}

FILE *__cstl_fwrite_all(FILE *fp, const char *buf) {
    char *filepath = fpm_get(fp);
    __cstl_fclose(fp);
    FILE *new_fp = __cstl_fopen(filepath, "wt");
    fpm_append(new_fp, filepath);
    if (!new_fp) return NULL;
    size_t len = strlen(buf);
    fwrite(buf, 1, len, new_fp) == len;
    return new_fp;
}

typedef FILE *(*cstl_fopen_fn)( char *filename, const char *mode);

typedef void (*cstl_fclose_fn)(FILE *fp);

typedef char *(*cstl_getline_fn)(FILE *fp);

typedef char *(*cstl_fread_all_fn)(FILE *fp);

typedef char *(*cstl_readall_fn)(FILE *fp, size_t *outsize);

typedef FILE * (*cstl_fwrite_all_fn)(FILE *fp, const char *buf);

typedef struct FSTREAM {
    cstl_fopen_fn open;
    cstl_fclose_fn close;
    cstl_getline_fn getline;
    cstl_fread_all_fn read;
    cstl_fwrite_all_fn write;
} FSTREAM;

FSTREAM fstream = {
    __cstl_fopen,
    __cstl_fclose,
    __cstl_get_line,
    __cstl_fread_all,
    __cstl_fwrite_all
};


#endif //_OPENCSTL_CSTL_FILE_H
