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
#ifndef OPENCSTL_JSON_H
#define OPENCSTL_JSON_H
//#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include "van_emde_boas_tree.h"
#if defined(OCSTL_CC_TCC) || defined(OCSTL_OS_LINUX)
// TCC는 strtok_s / strtok_r 둘 다 없으니 직접 구현
char *strtok_s(char *str, char *delimiters, char **last) {
    if (!delimiters || !last) return NULL;

    // 첫 호출이면 str에서, 재호출이면 저장된 위치에서 재개
    char *s = str ? str : *last;
    if (!s) return NULL;

    // 선행 delimiter 건너뛰기 (연속된 구분자는 하나로 취급)
    while (*s && strchr(delimiters, *s)) s++;
    if (!*s) {
        *last = NULL;
        return NULL;
    }

    // 토큰 시작 지점
    char *token = s;

    // 다음 delimiter까지 진행
    while (*s && !strchr(delimiters, *s)) s++;

    if (*s) {
        *s = '\0'; // delimiter를 NUL로 바꿔 토큰 종결
        *last = s + 1; // 다음 호출에서 여기부터 시작
    } else {
        *last = NULL; // 문자열 끝에 도달
    }
    return token;
}
#elif !defined(OCSTL_CC_MSVC)

#endif
typedef struct JSON_TOKEN JSON_TOKEN;
JSON_TOKEN *__g_last_token = NULL;

int as_int();

char *as_string();

bool as_bool();

double as_double();

bool is_null();

struct JSON_TOKEN {
    char *p;
    char *q;
    char key[512];
    char *str_buf; // as_string 결과 캐시
    struct JSON_TOKEN *children;
    struct JSON_TOKEN *next;

    int (*as_int)();

    bool (*as_bool)();

    char * (*as_string)();

    double (*as_double)();

    bool (*is_null)();
};

typedef struct JSON_TOKEN JSON;


int __as_int() {
    JSON_TOKEN *p = __g_last_token;
    if (!p || !p->p) return 0;
    return (int) strtol(p->p, NULL, 10);
}

char *__as_string() {
    JSON_TOKEN *p = __g_last_token;
    if (!p || !p->p || !p->q) return NULL;
    if (p->str_buf) return p->str_buf; // 이미 디코딩됨 → 캐시 반환

    char *start;
    char *end;
    if (*(p->p) == '"') {
        start = p->p + 1;
        end = p->q;
    } else {
        start = p->p;
        end = p->q + 1;
    }

    size_type64 cap = (size_type64) (end - start) + 1; // 이스케이프 처리 후 길이는 항상 원본 이하
    char *out = (char *) malloc(cap);
    if (!out) return NULL;

    char *dst = out;
    for (char *s = start; s < end; s++) {
        if (*s != '\\' || s + 1 >= end) {
            *dst++ = *s;
            continue;
        }
        s++;
        switch (*s) {
            case 'n': *dst++ = '\n';
                break;
            case 't': *dst++ = '\t';
                break;
            case 'r': *dst++ = '\r';
                break;
            case 'b': *dst++ = '\b';
                break;
            case 'f': *dst++ = '\f';
                break;
            case '"': *dst++ = '"';
                break;
            case '\\': *dst++ = '\\';
                break;
            case '/': *dst++ = '/';
                break;
            default: *dst++ = '\\';
                *dst++ = *s;
                break; // \uXXXX 등 미처리는 원본 유지
        }
    }
    *dst = '\0';

    p->str_buf = out;
    return out;
}

bool __as_bool() {
    JSON_TOKEN *p = __g_last_token;
    if (!p || !p->p) return false;
    return *(p->p) == 't'; // "true" 만 true, 나머지는 false
}

double __as_double() {
    JSON_TOKEN *p = __g_last_token;
    if (!p || !p->p) return 0.0;
    return strtod(p->p, NULL);
}

bool __is_null() {
    JSON_TOKEN *p = __g_last_token;
    return p && p->p && *(p->p) == 'n';
}

static char *__parse_value(char *s, JSON_TOKEN *node);

static char *__skip_ws(char *s) {
    while (*s && isspace((unsigned char) *s)) s++;
    return s;
}

static char *__parse_string(char *s, JSON_TOKEN *node) {
    node->p = s;
    s++;
    while (*s && *s != '"') {
        if (*s == '\\' && *(s + 1)) s++;
        s++;
    }
    node->q = s;
    return (*s == '"') ? s + 1 : s;
}

static char *__parse_number(char *s, JSON_TOKEN *node) {
    node->p = s;
    if (*s == '-' || *s == '+') s++;
    while (*s && (isdigit((unsigned char) *s) || *s == '.' ||
                  *s == 'e' || *s == 'E' || *s == '+' || *s == '-')) {
        s++;
    }
    node->q = s - 1;
    return s;
}

static char *__parse_literal(char *s, JSON_TOKEN *node) {
    node->p = s;
    while (*s && isalpha((unsigned char) *s)) s++;
    node->q = s - 1;
    return s;
}

static char *parse_array(char *s, JSON_TOKEN *node) {
    node->p = s;
    s++;
    s = __skip_ws(s);
    JSON_TOKEN **tail = &node->children;
    if (*s == ']') {
        node->q = s;
        return s + 1;
    }
    while (*s) {
        JSON_TOKEN *child = (JSON_TOKEN *) calloc(1, sizeof(JSON_TOKEN));
        s = __parse_value(s, child);
        *tail = child;
        tail = &child->next;
        s = __skip_ws(s);
        if (*s == ',') {
            s++;
            s = __skip_ws(s);
        } else if (*s == ']') {
            node->q = s;
            return s + 1;
        } else break;
    }
    node->q = s;
    return s;
}

static char *__parse_object(char *s, JSON_TOKEN *node) {
    node->p = s;
    s++;
    s = __skip_ws(s);
    JSON_TOKEN **tail = &node->children;
    if (*s == '}') {
        node->q = s;
        return s + 1;
    }

    while (*s) {
        s = __skip_ws(s);
        if (*s != '"') break;

        // 키의 시작/끝 기록
        s++; // 여는 " 스킵
        char *key_start = s;
        while (*s && *s != '"') {
            if (*s == '\\' && *(s + 1)) s++;
            s++;
        }
        char *key_end = s; // 닫는 " 위치
        if (*s == '"') s++;

        s = __skip_ws(s);
        if (*s != ':') break;
        s++;
        s = __skip_ws(s);

        JSON_TOKEN *child = (JSON_TOKEN *) calloc(1, sizeof(JSON_TOKEN));

        // 자식의 key 필드에 복사
        size_type64 klen = (size_type64) (key_end - key_start);
        if (klen > sizeof(child->key) - 1) klen = sizeof(child->key) - 1;
        memcpy(child->key, key_start, klen);
        child->key[klen] = '\0';

        s = __parse_value(s, child);
        *tail = child;
        tail = &child->next;

        s = __skip_ws(s);
        if (*s == ',') s++;
        else if (*s == '}') {
            node->q = s;
            return s + 1;
        } else break;
    }
    node->q = s;
    return s;
}

static char *__parse_value(char *s, JSON_TOKEN *node) {
    s = __skip_ws(s);
    if (*s == '{') return __parse_object(s, node);
    if (*s == '[') return parse_array(s, node);
    if (*s == '"') return __parse_string(s, node);
    if (*s == '-' || *s == '+' || isdigit((unsigned char) *s)) return __parse_number(s, node);
    if (isalpha((unsigned char) *s)) return __parse_literal(s, node);
    return s;
}

JSON_TOKEN *__parse(char *json_str) {
    JSON_TOKEN *root = (JSON_TOKEN *) calloc(1, sizeof(JSON_TOKEN));
    memset(root, 0, sizeof(JSON_TOKEN));
    __parse_value(json_str, root);
    bool iveb_init = false;
    if (iveb == NULL) {
        iveb = iveb_new();
        iveb_init = true;
    }
    iveb_insert(iveb, root, root, CT_JSON, 0, "json");
    if (iveb_init) {
        atexit(__opencstl_iveb_destroy);
    }
    return root;
}

JSON_TOKEN *__get(JSON_TOKEN *root, char *keys) {
    if (!root || !keys) return NULL;

    char buf[1024];
#ifdef _MSC_VER
    strncpy_s(buf, sizeof(buf), keys, _TRUNCATE);
#else
    strncpy(buf, keys, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
#endif

    JSON_TOKEN *cur = root;
    char *ctx = NULL;
    char *tok = strtok_s(buf, ".", &ctx); // 매크로 덕에 한 줄

    while (tok && cur) {
        JSON_TOKEN *found = NULL;

        if (cur->p && *(cur->p) == '[') {
            int idx = atoi(tok);
            JSON_TOKEN *c = cur->children;
            for (int i = 0; c && i < idx; i++) c = c->next;
            found = c;
        } else {
            for (JSON_TOKEN *c = cur->children; c; c = c->next) {
                if (strcmp(c->key, tok) == 0) {
                    found = c;
                    break;
                }
            }
        }

        if (!found) return NULL;
        cur = found;
        tok = strtok_s(NULL, ".", &ctx);
    }

    __g_last_token = cur;
    cur->as_int = __as_int;
    cur->as_bool = __as_bool;
    cur->as_string = __as_string;
    cur->as_double = __as_double;
    cur->is_null = __is_null;
    return cur;
}


static void __free_subtree(JSON_TOKEN *node) {
    if (!node) return;
    JSON_TOKEN *c = node->children;
    while (c) {
        JSON_TOKEN *nx = c->next;
        __free_subtree(c);
        c = nx;
    }
    free(node->str_buf); // as_string 캐시 해제
    free(node);
}

void __free_json(JSON *root) {
    if (!root) return;
    iveb_erase(iveb, root);
    JSON_TOKEN *c = root->children;
    while (c) {
        JSON_TOKEN *nx = c->next;
        __free_subtree(c);
        c = nx;
    }
    free(root->str_buf); // root에 as_string을 호출했을 수 있음
    root->str_buf = NULL;
    root->children = NULL;
    root->p = root->q = NULL;
    free(root);
}

static void __dumps(JSON_TOKEN *node, int depth) {
    if (!node || !node->p || !node->q) return;
    for (int i = 0; i < depth; i++) printf("  ");
    int len = (int) (node->q - node->p + 1);
    if (node->key[0]) printf("\"%s\": [%.*s]\n", node->key, len, node->p);
    else printf("[%.*s]\n", len, node->p);
    for (JSON_TOKEN *c = node->children; c; c = c->next) __dumps(c, depth + 1);
}

static void _dumps(JSON *root) {
    __dumps(root, 0);
}

typedef struct JSON_CLASS {
    JSON_TOKEN * (*parse)(char *json_str);

    JSON_TOKEN * (*get)(JSON_TOKEN *root, char *keys);

    void (*dumps)(JSON *root);

    //void (*delete)(JSON *root);
} JSON_CLASS;

JSON_CLASS json = {__parse, __get, _dumps};
#endif //OPENCSTL_JSON_H
