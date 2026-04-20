//
// Created by spring on 4/21/2026.
//

#ifndef OPENCSTL_FILEIO_H
#define OPENCSTL_FILEIO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crossplatform.h"
#if defined(OCSTL_OS_WINDOWS)
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#define FSTAT _fstat64
typedef struct __stat64 file_stat_t;
#else
#include <unistd.h>
#include <sys/stat.h>
#define FSTAT fstat
typedef struct stat file_stat_t;
#endif
typedef enum ENCODING{
    ENCODING_ANSI=0,
    ENCODING_UTF8=1,
}ENCODING;


typedef struct FILEIO{
    FILE* fp;
    ENCODING encoding;

}FILEIO;

char *fread_all(FILE *fp, size_t *out_size) {
    file_stat_t st;
    if (FSTAT(fileno(fp), &st) != 0) {
        return NULL;
    }

    if ((st.st_mode & S_IFMT) != S_IFREG) {
        return NULL;
    }

    size_t size = (size_t) st.st_size;
    char *buf = malloc(size + 1);
    if (!buf) return NULL;

    size_t n = fread(buf, 1, size, fp);
    buf[n] = '\0';
    if (out_size) *out_size = n;

    // 바이너리 모드라면 n == size가 보장됨
    // 텍스트 모드라면 n <= size 가능
    return buf;
}
wchar_t *fread_allw(FILE *fp, size_t *out_size) {
    file_stat_t st;
    if (FSTAT(fileno(fp), &st) != 0) {
        return NULL;
    }

    if ((st.st_mode & S_IFMT) != S_IFREG) {
        return NULL;
    }

    size_t size = (size_t) st.st_size;
    wchar_t *buf = malloc(size*sizeof(wchar_t) + 1);
    if (!buf) return NULL;

    size_t n = fread(buf, 2, size, fp);
    buf[n] = '\0';
    if (out_size) *out_size = n;

    // 바이너리 모드라면 n == size가 보장됨
    // 텍스트 모드라면 n <= size 가능
    return buf;
}
#endif //OPENCSTL_FILEIO_H
