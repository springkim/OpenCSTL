//
// Created by spring on 3/29/2026.
//

#if !defined(_OPENCSTL_CSTL_FILE_H)
#define _OPENCSTL_CSTL_FILE_H
#include "defines.h"
#include <stdio.h>


bool cstl_fopen(FILE **fp, const char *filename, const char *mode) {
#if defined(_WIN32) || defined(_WIN64)
    fopen_s(fp, filename, mode);
#else
    *fp = fopen(filename, mode);
#endif
    return *fp != NULL;
}

bool cstl_getline(FILE *fp, char *line, size_t size) {
    int c;
    size_t i = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') break;
        if (i + 1 < size)
            line[i++] = (char) c;
    }
    line[i] = '\0';
    return i > 0 || c != EOF;
}

char *cstl_fread_all(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buf = (char *) malloc(size);
    fread(buf, 1, size, fp);
    return buf;
}


#define FOPEN cstl_fopen
#define FCLOSE fclose
#define FGETLINE cstl_getline
#endif //_OPENCSTL_CSTL_FILE_H
