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
#ifndef OPENCSTL_FILESYSTEM_H
#define OPENCSTL_FILESYSTEM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "zalloc.h"
#include "tracer.h"
#include "crossplatform.h"
#if defined(OCSTL_OS_WINDOWS)
#if !defined(OCSTL_CC_TCC)
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#define CSTL_PATH_SEP '\\'
#else
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#define CSTL_PATH_SEP '/'
#endif
static bool __cstl_is_sep(char c) {
#if defined(OCSTL_OS_WINDOWS)
    return c == '/' || c == '\\';
#else
    return c == '/';
#endif
}

static bool __cstl_exists(char *path) {
#if defined(OCSTL_OS_WINDOWS)
    DWORD attrs = GetFileAttributesA(path);
    return attrs != INVALID_FILE_ATTRIBUTES;
#else
    struct stat st;
    return stat(path, &st) == 0;
#endif
}

static char *__cstl_join(char *path1, char *path2) {
    if (!path1) path1 = "";
    if (!path2) path2 = "";
    bool p2_abs = false;
    if (path2[0] != '\0') {
        if (__cstl_is_sep(path2[0])) p2_abs = true;
#if defined(OCSTL_OS_WINDOWS)
        if (isalpha((unsigned char) path2[0]) && path2[1] == ':') p2_abs = true;
#endif
    }
    if (p2_abs) {
        size_t l = strlen(path2);
        char *ret = (char *) malloc(l + 1);
        memcpy(ret, path2, l + 1);
        return ret;
    }
    size_t l1 = strlen(path1);
    size_t l2 = strlen(path2);
    if (l1 == 0) {
        char *ret = (char *) malloc(l2 + 1);
        memcpy(ret, path2, l2 + 1);
        return ret;
    }
    bool need_sep = !__cstl_is_sep(path1[l1 - 1]);
    size_t total = l1 + (need_sep ? 1 : 0) + l2;
    char *ret = (char *) malloc(total + 1);
    memcpy(ret, path1, l1);
    size_t pos = l1;
    if (need_sep) ret[pos++] = CSTL_PATH_SEP;
    memcpy(ret + pos, path2, l2);
    ret[pos + l2] = '\0';
    return ret;
}

static char *__cstl_basename(char *path) {
    size_t len = strlen(path);
    size_t start = 0;
    for (size_t i = 0; i < len; i++) {
        if (__cstl_is_sep(path[i])) start = i + 1;
    }
    size_t base_len = len - start;
    char *ret = (char *) malloc(base_len + 1);
    memcpy(ret, path + start, base_len);
    ret[base_len] = '\0';
    return ret;
}

static char **__cstl_splitext(char *path) {
    size_t len = strlen(path);
    size_t base_start = 0;
    for (size_t i = 0; i < len; i++) {
        if (__cstl_is_sep(path[i])) base_start = i + 1;
    }
    size_t nonleading = base_start;
    while (nonleading < len && path[nonleading] == '.') nonleading++;
    size_t dot_pos = (size_t) -1;
    for (size_t i = nonleading; i < len; i++) {
        if (path[i] == '.') dot_pos = i;
    }
    size_t root_len = (dot_pos == (size_t) -1) ? len : dot_pos;
    size_t ext_len = (dot_pos == (size_t) -1) ? 0 : (len - dot_pos);
    size_t total = 2 * sizeof(char *) + root_len + 1 + ext_len + 1;
    char **ret = (char **) malloc(total);
    char *buf = (char *) ret + 2 * sizeof(char *);
    memcpy(buf, path, root_len);
    buf[root_len] = '\0';
    memcpy(buf + root_len + 1, path + root_len, ext_len);
    buf[root_len + 1 + ext_len] = '\0';
    ret[0] = buf;
    ret[1] = buf + root_len + 1;
    return ret;
}

static size_t __cstl_getsize(char *path) {
#if defined(OCSTL_OS_WINDOWS)
    WIN32_FILE_ATTRIBUTE_DATA attr;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &attr)) return 0;
    ULARGE_INTEGER sz;
    sz.LowPart = attr.nFileSizeLow;
    sz.HighPart = attr.nFileSizeHigh;
    return (size_t) sz.QuadPart;
#else
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return (size_t) st.st_size;
#endif
}

static void __cstl_makedirs(char *path) {
    size_t len = strlen(path);
    if (len == 0) return;
    char *tmp = (char *) malloc(len + 1);
    memcpy(tmp, path, len + 1);
    for (size_t i = 1; i < len; i++) {
        if (__cstl_is_sep(tmp[i])) {
            char save = tmp[i];
            tmp[i] = '\0';
#if defined(OCSTL_OS_WINDOWS)
            CreateDirectoryA(tmp, NULL);
#else
            mkdir(tmp, 0755);
#endif
            tmp[i] = save;
        }
    }
#if defined(OCSTL_OS_WINDOWS)
    CreateDirectoryA(tmp, NULL);
#else
    mkdir(tmp, 0755);
#endif
    free(tmp);
}

static void __cstl_remove(char *path) {
#if defined(OCSTL_OS_WINDOWS)
    DeleteFileA(path);
#else
    unlink(path);
#endif
}

static void __cstl_rename(char *oldpath, char *newpath) {
    rename(oldpath, newpath);
}

static char *__cstl_dirname(char *path) {
    size_t len = strlen(path);
    size_t last_sep = (size_t) -1;
    for (size_t i = 0; i < len; i++) {
        if (__cstl_is_sep(path[i])) last_sep = i;
    }
    if (last_sep == (size_t) -1) {
        char *ret = (char *) malloc(1);
        ret[0] = '\0';
        return ret;
    }
    size_t dir_len = last_sep;
    if (dir_len == 0) dir_len = 1;
#if defined(OCSTL_OS_WINDOWS)
    if (last_sep == 2 && isalpha((unsigned char) path[0]) && path[1] == ':') {
        dir_len = 3;
    }
#endif
    char *ret = (char *) malloc(dir_len + 1);
    memcpy(ret, path, dir_len);
    ret[dir_len] = '\0';
    return ret;
}

static char *__cstl_abspath(char *path) {
    if (!path) path = "";
#if defined(OCSTL_OS_WINDOWS)
    DWORD len = GetFullPathNameA(path, 0, NULL, NULL);
    if (len == 0) {
        char *ret = (char *) malloc(strlen(path) + 1);
        memcpy(ret, path, strlen(path) + 1);
        return ret;
    }
    char *ret = (char *) malloc(len);
    GetFullPathNameA(path, len, ret, NULL);
    return ret;
#else
    char resolved[PATH_MAX];
    if (realpath(path, resolved)) {
        size_t l = strlen(resolved);
        char *ret = (char *) malloc(l + 1);
        memcpy(ret, resolved, l + 1);
        return ret;
    }
    if (path[0] == '/') {
        size_t l = strlen(path);
        char *ret = (char *) malloc(l + 1);
        memcpy(ret, path, l + 1);
        return ret;
    }
    char cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof cwd)) {
        size_t l = strlen(path);
        char *ret = (char *) malloc(l + 1);
        memcpy(ret, path, l + 1);
        return ret;
    }
    return __cstl_join(cwd, path);
#endif
}

static bool __cstl_is_dir(char *path) {
#if defined(OCSTL_OS_WINDOWS)
    DWORD a = GetFileAttributesA(path);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return S_ISDIR(st.st_mode);
#endif
}

static bool __cstl_is_file(char *path) {
#if defined(OCSTL_OS_WINDOWS)
    DWORD a = GetFileAttributesA(path);
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return S_ISREG(st.st_mode);
#endif
}

typedef bool (*cstl_exists_fn)(char *path);

typedef char *(*cstl_join_fn)(char *path1, char *path2);

typedef char *(*cstl_basename_fn)(char *path);

typedef char **(*cstl_splitext_fn)(char *path);

typedef size_t (*cstl_getsize_fn)(char *path);

typedef void (*cstl_makedirs_fn)(char *path);

typedef void (*cstl_remove_fn)(char *path);

typedef void (*cstl_rename_fn)(char *oldpath, char *newpath);

typedef char *(*cstl_dirname_fn)(char *path);

typedef char *(*cstl_abspath_fn)(char *path);

typedef bool (*cstl_is_dir_fn)(char *path);

typedef bool (*cstl_is_file_fn)(char *path);

typedef struct {
    cstl_exists_fn exists;
    cstl_join_fn join;
    cstl_basename_fn basename;
    cstl_splitext_fn splitext;
    cstl_getsize_fn getsize;
    cstl_makedirs_fn makedirs;
    cstl_remove_fn remove;
    cstl_rename_fn rename;
    cstl_dirname_fn dirname;
    cstl_abspath_fn abspath;
    cstl_is_dir_fn is_dir;
    cstl_is_file_fn is_file;
} FileSystem;

static FileSystem fs = {
    __cstl_exists,
    __cstl_join,
    __cstl_basename,
    __cstl_splitext,
    __cstl_getsize,
    __cstl_makedirs,
    __cstl_remove,
    __cstl_rename,
    __cstl_dirname,
    __cstl_abspath,
    __cstl_is_dir,
    __cstl_is_file,
};
#endif // OPENCSTL_FILESYSTEM_H
