//
// Created by spring on 4/24/2026.
//

#ifndef OPENCSTL_CSV_H
#define OPENCSTL_CSV_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "van_emde_boas_tree.h"
#include "verify.h"
#include"file.h"

typedef struct {
    char ***table;
    char **header;
    int rows; // height
    int cols; // width
} CSV;

// dst == NULL이면 길이만 측정 (pass 1), dst != NULL이면 기록 (pass 2)
static const char *csv__next_field(const char *p, const char *end,
                                   char *dst, size_t *out_len, int *eol) {
    size_t len = 0;
    *eol = 0;
    if (p >= end) {
        *out_len = 0;
        *eol = 1;
        return p;
    }

    if (*p == '"') {
        p++;
        while (p < end) {
            if (*p == '"') {
                if (p + 1 < end && *(p + 1) == '"') {
                    // escaped quote
                    if (dst) dst[len] = '"';
                    len++;
                    p += 2;
                } else {
                    p++;
                    break;
                }
            } else {
                if (dst) dst[len] = *p;
                len++;
                p++;
            }
        }
    } else {
        while (p < end && *p != ',' && *p != '\r' && *p != '\n') {
            if (dst) dst[len] = *p;
            len++;
            p++;
        }
    }
    if (dst) dst[len] = '\0';
    *out_len = len;

    if (p >= end) { *eol = 1; } else if (*p == '\r') {
        p++;
        if (p < end && *p == '\n') p++;
        *eol = 1;
    } else if (*p == '\n') {
        p++;
        *eol = 1;
    } else if (*p == ',') { p++; }

    return p;
}

static CSV __parse_csv(char *csv_path, bool is_header) {
    CSV ret;
    ret.table = NULL;
    ret.header = NULL;
    ret.rows = 0;
    ret.cols = 0;
    // ret.table에 데이터 저장
    // is_header가 true이면 header에 cols 길이만큼 문자열 배열로 할당.
    // is_header가 false이면 NULL,
    // rows는 헤더를 제외한 행 개수

    // ── 파일 전체를 한 번에 읽기 ─────────────────────────────────────────
    FILE *f = file.open(csv_path, "rb");
    if (!f) return ret;
    fseek(f, 0, SEEK_END);
    long fsz = ftell(f);
    rewind(f);
    verify(fsz > 0);




    char *buf = (char *) malloc((size_t) fsz);
    verify(buf!=NULL);

    fread(buf, 1, (size_t) fsz, f);
    file.close(f);

    const char *end = buf + fsz;

    // ── Pass 1: 행 수 / 열 수 / 문자열 바이트 합산 ──────────────────────
    int total_rows = 0;
    int max_cols = 0;
    size_t str_bytes = 0;
    int row_cap = 64;
    int *row_cols = (int *) malloc(row_cap * sizeof(int));
    verify(row_cols!=NULL);


    const char *p = buf;
    while (p < end) {
        if (total_rows == row_cap) {
            row_cap *= 2;
            int *tmp = (int *) realloc(row_cols, row_cap * sizeof(int));
            verify(tmp!=NULL);
            row_cols = tmp;
        }
        int ncols = 0;
        int eol = 0;
        while (!eol) {
            size_t flen;
            const char *next = csv__next_field(p, end, NULL, &flen, &eol);
            if (next == p) goto done_pass1;
            str_bytes += flen + 1; // +1 for '\0'
            ncols++;
            p = next;
        }
        if (ncols == 0) break;
        row_cols[total_rows++] = ncols;
        if (ncols > max_cols) max_cols = ncols;
    }
done_pass1:;

    // 짧은 행 패딩용 빈 문자열 공간
    for (int i = 0; i < total_rows; i++)
        if (row_cols[i] < max_cols)
            str_bytes += (size_t) (max_cols - row_cols[i]);

    // ── 단일 블록 레이아웃 ────────────────────────────────────────────────
    // [ char**  row_ptrs  : data_rows * sizeof(char**)         ]  ← table[i]
    // [ char*   cell_ptrs : data_rows * max_cols * sizeof(char*)]  ← table[i][j]
    // [ char*   hdr_ptrs  : max_cols * sizeof(char*)  (헤더 시) ]  ← header[j]
    // [ char    sdata     : 가변 문자열 밀집 패킹               ]

    int data_rows = is_header
                        ? (total_rows > 0 ? total_rows - 1 : 0)
                        : total_rows;
    size_t off1 = (size_t) data_rows * sizeof(char **);
    size_t off2 = (size_t) data_rows * (size_t) max_cols * sizeof(char *);
    size_t off3 = is_header ? (size_t) max_cols * sizeof(char *) : 0;

    char *mem = (char *) malloc(off1 + off2 + off3 + str_bytes);
    verify(mem!=NULL);

    char ***table = (char ***) mem;
    char **cells = (char **) (mem + off1);
    char **hdr = is_header ? (char **) (mem + off1 + off2) : NULL;
    char *sdata = mem + off1 + off2 + off3;

    // row_ptrs 세팅
    for (int i = 0; i < data_rows; i++)
        table[i] = cells + i * max_cols;

    // ── Pass 2: 문자열 기록 + 포인터 세팅 ───────────────────────────────
    p = buf;
    char *sp = sdata;

    // 헤더 행 처리
    if (is_header && total_rows > 0) {
        int j = 0, eol = 0;
        while (!eol && j < row_cols[0]) {
            size_t flen;
            hdr[j] = sp;
            p = csv__next_field(p, end, sp, &flen, &eol);
            sp += flen + 1;
            j++;
        }
        for (; j < max_cols; j++) {
            hdr[j] = sp;
            *sp++ = '\0';
        } // 패딩
    }

    // 데이터 행 처리
    int start = is_header ? 1 : 0;
    for (int i = start; i < total_rows; i++) {
        int di = i - start, j = 0, eol = 0;
        while (!eol && j < row_cols[i]) {
            size_t flen;
            table[di][j] = sp;
            p = csv__next_field(p, end, sp, &flen, &eol);
            sp += flen + 1;
            j++;
        }
        for (; j < max_cols; j++) {
            table[di][j] = sp;
            *sp++ = '\0';
        } // 패딩
    }

    free(row_cols);
    free(buf);

    ret.table = table;
    ret.header = hdr;
    ret.rows = data_rows;
    ret.cols = max_cols;
    bool iveb_init = false;
    if (iveb == NULL) {
        iveb = iveb_new();
        iveb_init = true;
    }
    iveb_insert(iveb, table, table, CT_CSV, 0, "csv");
    if (iveb_init) {
        atexit(__opencstl_iveb_destroy);
    }
    return ret;
}

// free는 블록 하나만 해제하면 끝
void __free_csv(CSV *csv) {
    iveb_erase(iveb, csv->table);
    free(csv->table);
    csv->table = NULL;
    csv->header = NULL;
    csv->rows = 0;
    csv->cols = 0;
}

typedef struct CSV_CLASS {
    CSV (*parse)(char *csv_path,bool is_header);
} CSV_CLASS;

CSV_CLASS csv = {
    __parse_csv
};
#endif //OPENCSTL_CSV_H
