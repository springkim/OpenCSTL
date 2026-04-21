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

#if !defined(OPENCSTL_MSG_H)
#define OPENCSTL_MSG_H
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <stdarg.h>

static void ConsoleMSG(const char *format, ...) {
#if defined(OCSTL_CC_MSVC)
    SetConsoleOutputCP(CP_UTF8);
#elif defined(OCSTL_CC_TCC)
    setvbuf(stdout, NULL, _IONBF, 0); // unbuffered — 바이트 단위 즉시 전송
#endif
    int width = 80;
    setlocale(LC_ALL, ".UTF-8");

    va_list args;
    va_start(args, format);
    char buf[1024];
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    const char *tl = "\xE2\x94\x8F"; // ┏
    const char *tr = "\xE2\x94\x93"; // ┓
    const char *bl = "\xE2\x94\x97"; // ┗
    const char *br = "\xE2\x94\x9B"; // ┛
    const char *vline = "\xE2\x94\x81"; // ━ (가로선)
    const char *hline = "\xE2\x94\x83"; // ┃ (세로선)

    int inner = width - 2;

    // 상단
    fputs(tl, stdout);
    for (int i = 0; i < inner; i++) fputs(vline, stdout);
    fputs(tr, stdout);
    putchar('\n');

    // 중간 — 각 라인을 개별 중앙 정렬
    const char *line_start = buf;
    const char *p = buf;
    while (1) {
        if (*p == '\n' || *p == '\0') {
            int len = (int) (p - line_start);
            int lpadding = (inner - len) / 2;
            int rpadding = inner - len - lpadding;
            if (lpadding < 0) lpadding = 0;
            if (rpadding < 0) rpadding = 0;

            fputs(hline, stdout);
            for (int i = 0; i < lpadding; i++) putchar(' ');
            fwrite(line_start, 1, (size_t) len, stdout);
            for (int i = 0; i < rpadding; i++) putchar(' ');
            fputs(hline, stdout);
            putchar('\n');

            if (*p == '\0') break;
            line_start = p + 1;
        }
        p++;
    }

    // 하단
    fputs(bl, stdout);
    for (int i = 0; i < inner; i++) fputs(vline, stdout);
    fputs(br, stdout);
    putchar('\n');
}
#endif //OPENCSTL_MSG_H
