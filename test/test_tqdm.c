//
// Created by spring on 4/21/2026.
//

// cstl_tqdm.h - minimal ASCII progress bar
// C99+, no deps. \b 방식으로 이전 줄을 덮어씀 (ANSI escape 미사용).
// non-TTY (파일 리디렉션, 일부 IDE Run 창)에선 줄바꿈 모드로 자동 전환.

#ifndef CSTL_TQDM_H
#define CSTL_TQDM_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <io.h>
#  define tqdm_isatty_(fd)  _isatty(fd)
#  define tqdm_fileno_(fp)  _fileno(fp)
#else
#  include <time.h>
#  include <unistd.h>
#  define tqdm_isatty_(fd)  isatty(fd)
#  define tqdm_fileno_(fp)  fileno(fp)
#endif

#include "../opencstl/opencstl.h"

typedef struct {
    size_type64 total;             // 0이면 unknown (indeterminate bar)
    size_type64 current;
    int    bar_width;
    int    mininterval_ms;

    double t0;
    double t_last_print;
    double t_last_sample;
    size_type64 n_last_sample;

    double ema_rate;
    double smoothing;

    int    last_line_len;     // 이전 render에서의 커서 위치 (= 출력 길이)
    int    is_tty;

    const char *desc;
    FILE *fp;
} tqdm_t;

static inline double tqdm_now_(void) {
#if defined(_WIN32)
    static LARGE_INTEGER freq;
    LARGE_INTEGER t;
    if (!freq.QuadPart) QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t);
    return (double)t.QuadPart / (double)freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
#endif
}

static inline void tqdm_fmt_time_(double sec, char *buf, size_type64 bufsz) {
    if (!(sec >= 0.0) || sec > 359999.0) {
        snprintf(buf, bufsz, "??:??");
        return;
    }
    long total = (long)sec;
    int h = (int)(total / 3600);
    int m = (int)((total % 3600) / 60);
    int s = (int)(total % 60);
    if (h > 0) snprintf(buf, bufsz, "%d:%02d:%02d", h, m, s);
    else       snprintf(buf, bufsz, "%02d:%02d", m, s);
}

static inline void tqdm_init(tqdm_t *t, size_type64 total) {
    t->total = total;
    t->current = 0;
    t->bar_width = 30;
    t->t0 = tqdm_now_();
    t->t_last_print = 0.0;
    t->t_last_sample = t->t0;
    t->n_last_sample = 0;
    t->ema_rate = 0.0;
    t->smoothing = 0.3;
    t->last_line_len = 0;
    t->desc = NULL;
    t->fp = stderr;

    // TTY 감지
    t->is_tty = tqdm_isatty_(tqdm_fileno_(t->fp)) ? 1 : 0;

    // override: 컴파일 플래그 or 환경변수로 강제 TTY 모드
#if defined(TQDM_ASSUME_TTY)
    t->is_tty = 1;
#else
    {
        const char *env = getenv("TQDM_FORCE_TTY");
        if (env && env[0] == '1') t->is_tty = 1;
    }
#endif

    t->mininterval_ms = t->is_tty ? 100 : 1000;

    // stderr unbuffered: IDE pipe 상황에서 flush 누락 방지
    setvbuf(t->fp, NULL, _IONBF, 0);
}

static inline void tqdm_render_(tqdm_t *t, int final_) {
    double now = tqdm_now_();
    double elapsed = now - t->t0;

    double dt = now - t->t_last_sample;
    if (dt >= 0.05) {
        double inst = (double)(t->current - t->n_last_sample) / dt;
        t->ema_rate = (t->ema_rate <= 0.0)
            ? inst
            : t->smoothing * inst + (1.0 - t->smoothing) * t->ema_rate;
        t->t_last_sample = now;
        t->n_last_sample = t->current;
    }

    double rate = t->ema_rate;
    if (rate <= 0.0 && elapsed > 0.0) rate = (double)t->current / elapsed;

    char e_buf[16], r_buf[16];
    tqdm_fmt_time_(elapsed, e_buf, sizeof e_buf);

    char line[512];
    int len = 0;
    int n;

    if (t->desc) {
        n = snprintf(line + len, sizeof line - len, "%s: ", t->desc);
        if (n > 0) len += n;
    }

    if (t->total > 0) {
        size_type64 remaining = (t->current >= t->total) ? 0 : (t->total - t->current);
        double eta = (rate > 0.0) ? (double)remaining / rate : -1.0;
        tqdm_fmt_time_(eta, r_buf, sizeof r_buf);

        int pct = (int)((double)t->current * 100.0 / (double)t->total);
        if (pct > 100) pct = 100;
        int filled = (int)((double)t->current * (double)t->bar_width / (double)t->total);
        if (filled > t->bar_width) filled = t->bar_width;

        n = snprintf(line + len, sizeof line - len, "%3d%%|", pct);
        if (n > 0) len += n;
        for (int i = 0; i < filled            && len < (int)sizeof line - 1; i++) line[len++] = '#';
        for (int i = filled; i < t->bar_width && len < (int)sizeof line - 1; i++) line[len++] = ' ';
        n = snprintf(line + len, sizeof line - len, "| %zu/%zu [%s<%s, %.2fit/s]",
                     t->current, t->total, e_buf, r_buf, rate);
        if (n > 0) len += n;
    } else {
        n = snprintf(line + len, sizeof line - len, "%zuit [%s, %.2fit/s]",
                     t->current, e_buf, rate);
        if (n > 0) len += n;
    }

    // Python tqdm 방식: 분기 없이 항상 \r만 쓰고 \n은 close에서만
    fputc('\r', t->fp);
    fwrite(line, 1, (size_type64)len, t->fp);
    int extra = t->last_line_len - len;
    for (int i = 0; i < extra; i++) fputc(' ', t->fp);
    for (int i = 0; i < extra; i++) fputc('\b', t->fp);
    t->last_line_len = len;

    if (final_) {
        fputc('\n', t->fp);
        t->last_line_len = 0;
    }
    fflush(t->fp);
    t->t_last_print = now;
}
static inline void tqdm_update(tqdm_t *t, size_type64 n) {
    t->current += n;
    double now = tqdm_now_();
    double since_ms = (now - t->t_last_print) * 1000.0;
    int done = (t->total > 0 && t->current >= t->total);
    if (since_ms >= 100.0 || done) {
        tqdm_render_(t, done);
    }
}

static inline void tqdm_close(tqdm_t *t) {
    tqdm_render_(t, 1);
}

#endif  // CSTL_TQDM_H


#if defined(_WIN32)
#  include <windows.h>
static void sleep_us(int us) { Sleep((us + 999) / 1000); }
#else
#  include <time.h>
static void sleep_us(int us) {
    struct timespec ts = { us / 1000000, (us % 1000000) * 1000L };
    nanosleep(&ts, NULL);
}
#endif

int main(void) {
    tqdm_t pbar;
    tqdm_init(&pbar, 100000);   // 1000은 너무 빨라서 bar를 못 봄
    pbar.bar_width = 30;

    for (size_type64 i = 0; i < pbar.total; i++) {
        sleep_us(100);
        tqdm_update(&pbar, 1);
    }
    tqdm_close(&pbar);
    return 0;
}