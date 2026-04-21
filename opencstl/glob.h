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


#ifndef OPENCSTL_GLOB_H
#define OPENCSTL_GLOB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "crossplatform.h"

#if defined(OCSTL_OS_WINDOWS)
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

// ────────────────────────────────────────────────────────────
// Filesystem helpers
// ────────────────────────────────────────────────────────────

static bool __cstl_glob_is_sep(char c) {
#if defined(OCSTL_OS_WINDOWS)
    return c == '/' || c == '\\';
#else
    return c == '/';
#endif
}

static bool __cstl_glob_exists_(const char *path) {
#if defined(OCSTL_OS_WINDOWS)
    return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
#else
    struct stat st;
    return stat(path, &st) == 0;
#endif
}

static bool __cstl_glob_isdir_(const char *path) {
#if defined(OCSTL_OS_WINDOWS)
    DWORD a = GetFileAttributesA(path);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return S_ISDIR(st.st_mode);
#endif
}

static char *__cstl_glob_join_(const char *a, const char *b) {
    size_t la = strlen(a), lb = strlen(b);
    bool need_sep = la > 0 && !__cstl_glob_is_sep(a[la - 1]);
    char *r = (char *) malloc(la + (need_sep ? 1 : 0) + lb + 1);
    memcpy(r, a, la);
    size_t pos = la;
    if (need_sep) {
        r[pos++] = '/';
    }
    memcpy(r + pos, b, lb);
    r[pos + lb] = '\0';
    return r;
}

// Returns malloc'd char** (each entry malloc'd). "." and ".." excluded.
static char **__cstl_glob_listdir_(const char *path, int *n_out) {
    int cap = 16, cnt = 0;
    char **names = (char **) malloc(cap * sizeof(char *));
    *n_out = 0;

#if defined(OCSTL_OS_WINDOWS)
    size_t plen = strlen(path);
    char *pat = (char *) malloc(plen + 3);
    memcpy(pat, path, plen);
    size_t pos = plen;
    if (plen > 0 && !__cstl_glob_is_sep(path[plen - 1])) pat[pos++] = '\\';
    pat[pos++] = '*';
    pat[pos] = '\0';

    WIN32_FIND_DATAA data;
    HANDLE h = FindFirstFileA(pat, &data);
    free(pat);
    if (h == INVALID_HANDLE_VALUE) return names;
    do {
        if (strcmp(data.cFileName, ".") == 0 || strcmp(data.cFileName, "..") == 0) continue;
        if (cnt >= cap) { cap *= 2; names = (char **) realloc(names, cap * sizeof(char *)); }
        size_t nl = strlen(data.cFileName);
        names[cnt] = (char *) malloc(nl + 1);
        memcpy(names[cnt], data.cFileName, nl + 1);
        cnt++;
    } while (FindNextFileA(h, &data));
    FindClose(h);
#else
    DIR *d = opendir(path);
    if (!d) return names;
    struct dirent *e;
    while ((e = readdir(d))) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;
        if (cnt >= cap) { cap *= 2; names = (char **) realloc(names, cap * sizeof(char *)); }
        size_t nl = strlen(e->d_name);
        names[cnt] = (char *) malloc(nl + 1);
        memcpy(names[cnt], e->d_name, nl + 1);
        cnt++;
    }
    closedir(d);
#endif

    *n_out = cnt;
    return names;
}

// ────────────────────────────────────────────────────────────
// Internal: single-segment fnmatch (used by walker)
// ────────────────────────────────────────────────────────────

static bool __cstl_glob_fnmatch_(const char *pat, const char *name) {
    while (*pat) {
        if (*pat == '*') {
            while (*pat == '*') pat++;
            if (!*pat) return true;
            while (*name) {
                if (__cstl_glob_fnmatch_(pat, name)) return true;
                name++;
            }
            return false;
        }
        if (!*name) return false;

        if (*pat == '?') {
            pat++; name++;
        } else if (*pat == '[') {
            const char *p = pat + 1;
            bool negate = false;
            if (*p == '!' || *p == '^') { negate = true; p++; }
            bool matched = false;
            if (*p == ']') {
                if (*name == ']') matched = true;
                p++;
            }
            while (*p && *p != ']') {
                if (p[1] == '-' && p[2] && p[2] != ']') {
                    unsigned char lo = (unsigned char) p[0], hi = (unsigned char) p[2];
                    unsigned char c = (unsigned char) *name;
                    if (c >= lo && c <= hi) matched = true;
                    p += 3;
                } else {
                    if (*name == *p) matched = true;
                    p++;
                }
            }
            if (*p != ']') return false;
            pat = p + 1;
            if (matched == negate) return false;
            name++;
        } else {
            if (*pat != *name) return false;
            pat++; name++;
        }
    }
    return !*name;
}

static bool __cstl_glob_has_magic_(const char *s) {
    for (; *s; s++) {
        if (*s == '*' || *s == '?' || *s == '[') return true;
    }
    return false;
}

// ────────────────────────────────────────────────────────────
// Pattern parse: split "pattern" into base dir + segment list
// ────────────────────────────────────────────────────────────

static void __cstl_glob_parse_(const char *pat,
                               char **base_out, char ***segs_out, int *n_out) {
    size_t plen = strlen(pat);
    const char *rest = pat;
    char *base;

#if defined(OCSTL_OS_WINDOWS)
    if (plen >= 2 && isalpha((unsigned char) pat[0]) && pat[1] == ':') {
        if (plen >= 3 && __cstl_glob_is_sep(pat[2])) {
            base = (char *) malloc(4);
            base[0] = pat[0]; base[1] = ':'; base[2] = '/'; base[3] = '\0';
            rest = pat + 3;
        } else {
            base = (char *) malloc(3);
            base[0] = pat[0]; base[1] = ':'; base[2] = '\0';
            rest = pat + 2;
        }
    } else if (plen >= 1 && __cstl_glob_is_sep(pat[0])) {
        base = (char *) malloc(2);
        base[0] = '/'; base[1] = '\0';
        rest = pat + 1;
    } else {
        base = (char *) malloc(2);
        base[0] = '.'; base[1] = '\0';
    }
#else
    if (plen >= 1 && pat[0] == '/') {
        base = (char *) malloc(2);
        base[0] = '/'; base[1] = '\0';
        rest = pat + 1;
    } else {
        base = (char *) malloc(2);
        base[0] = '.'; base[1] = '\0';
    }
#endif

    int cap = 8, n = 0;
    char **segs = (char **) malloc(cap * sizeof(char *));

    const char *start = rest;
    for (const char *p = rest; ; p++) {
        if (__cstl_glob_is_sep(*p) || *p == '\0') {
            size_t sl = (size_t) (p - start);
            if (sl > 0) {
                if (n >= cap) { cap *= 2; segs = (char **) realloc(segs, cap * sizeof(char *)); }
                segs[n] = (char *) malloc(sl + 1);
                memcpy(segs[n], start, sl);
                segs[n][sl] = '\0';
                n++;
            }
            if (*p == '\0') break;
            start = p + 1;
        }
    }

    *base_out = base;
    *segs_out = segs;
    *n_out = n;
}

// ────────────────────────────────────────────────────────────
// Walker
// ────────────────────────────────────────────────────────────

typedef struct {
    char **items;
    int cnt;
    int cap;
} __cstl_glob_result_;

static void __cstl_glob_push_(__cstl_glob_result_ *r, const char *s) {
    // +1 for NULL terminator
    if (r->cnt + 1 >= r->cap) {
        r->cap = r->cap ? r->cap * 2 : 16;
        r->items = (char **) realloc(r->items, r->cap * sizeof(char *));
    }
    size_t l = strlen(s);
    r->items[r->cnt] = (char *) malloc(l + 1);
    memcpy(r->items[r->cnt], s, l + 1);
    r->cnt++;
}

static void __cstl_glob_walk_(const char *base,
                              char **segs, int i, int n,
                              bool recursive,
                              __cstl_glob_result_ *out) {
    if (i >= n) {
        if (__cstl_glob_exists_(base)) __cstl_glob_push_(out, base);
        return;
    }
    const char *seg = segs[i];

    // ** — zero or more directories (recursive only)
    if (recursive && strcmp(seg, "**") == 0) {
        __cstl_glob_walk_(base, segs, i + 1, n, recursive, out);
        int dn;
        char **dnames = __cstl_glob_listdir_(base, &dn);
        for (int k = 0; k < dn; k++) {
            if (dnames[k][0] != '.') {
                char *sub = __cstl_glob_join_(base, dnames[k]);
                if (__cstl_glob_isdir_(sub)) {
                    __cstl_glob_walk_(sub, segs, i, n, recursive, out);
                }
                free(sub);
            }
            free(dnames[k]);
        }
        free(dnames);
        return;
    }

    bool is_last = (i == n - 1);

    if (!__cstl_glob_has_magic_(seg)) {
        char *full = __cstl_glob_join_(base, seg);
        if (is_last) {
            if (__cstl_glob_exists_(full)) __cstl_glob_push_(out, full);
        } else if (__cstl_glob_isdir_(full)) {
            __cstl_glob_walk_(full, segs, i + 1, n, recursive, out);
        }
        free(full);
        return;
    }

    int dn;
    char **dnames = __cstl_glob_listdir_(base, &dn);
    bool pat_starts_dot = (seg[0] == '.');
    for (int k = 0; k < dn; k++) {
        if (dnames[k][0] == '.' && !pat_starts_dot) {
            free(dnames[k]);
            continue;
        }
        if (__cstl_glob_fnmatch_(seg, dnames[k])) {
            char *full = __cstl_glob_join_(base, dnames[k]);
            if (is_last) {
                __cstl_glob_push_(out, full);
            } else if (__cstl_glob_isdir_(full)) {
                __cstl_glob_walk_(full, segs, i + 1, n, recursive, out);
            }
            free(full);
        }
        free(dnames[k]);
    }
    free(dnames);
}

// ────────────────────────────────────────────────────────────
// Public API
// ────────────────────────────────────────────────────────────

// Returns a NULL-terminated char** array. Iterate until *p == NULL.
// Free with glob_free().
static char **__cstl_glob_impl_(const char *pattern, bool recursive) {
    char *base;
    char **segs;
    int n_segs;
    __cstl_glob_parse_(pattern, &base, &segs, &n_segs);

    __cstl_glob_result_ r = {0};
    __cstl_glob_walk_(base, segs, 0, n_segs, recursive, &r);

    for (int i = 0; i < n_segs; i++) free(segs[i]);
    free(segs);
    free(base);

    // Ensure space for NULL sentinel
    if (!r.items) {
        r.items = (char **) malloc(sizeof(char *));
    } else if (r.cnt + 1 > r.cap) {
        r.items = (char **) realloc(r.items, (r.cnt + 1) * sizeof(char *));
    }
    r.items[r.cnt] = NULL;

    // Normalize paths: backslash → forward slash, strip leading "./"
    for (int i = 0; i < r.cnt; i++) {
        char *s = r.items[i];
        for (char *p = s; *p; p++) {
            if (*p == '\\') *p = '/';
        }
        if (s[0] == '.' && s[1] == '/') {
            size_t l = strlen(s);
            memmove(s, s + 2, l - 1); // includes '\0'
        }
    }
    return r.items;
}

static void glob_free(char **results) {
    if (!results) return;
    for (char **p = results; *p; p++) free(*p);
    free(results);
}

// Overload dispatch: glob(p) or glob(p, recursive)
#define __CSTL_GLOB_PICK(_1, _2, NAME, ...) NAME
#define __CSTL_GLOB_1(p)    __cstl_glob_impl_((p), false)
#define __CSTL_GLOB_2(p, r) __cstl_glob_impl_((p), (r))
#define glob(...) __CSTL_GLOB_PICK(__VA_ARGS__, __CSTL_GLOB_2, __CSTL_GLOB_1)(__VA_ARGS__)

#endif // OPENCSTL_GLOB_H