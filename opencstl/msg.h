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

static void MsgBoxCLI(const char *format, ...) {
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
            fwrite(line_start, 1, (size_type64) len, stdout);
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

#if defined(OCSTL_OS_WINDOWS)

#pragma comment(lib, "user32.lib")
static char *_MsgTitle(void) {
    static char buf[1024] = {0};

#if defined(OCSTL_CC_GCC)
sprintf(buf, "%s", "GNU gcc Runtime Library");
#elif defined(OCSTL_CC_MSVC)
sprintf(buf, "%s", "Microsoft Visual C Runtime Library");
#elif defined(OCSTL_CC_CLANG)
sprintf(buf, "%s", "LLVM clang Runtime Library");
#elif defined(OCSTL_CC_TCC)
sprintf(buf, "%s", "TCC Runtime Library");
#endif
return buf;
}

typedef int (WINAPI *PFN_MessageBoxExA)(HWND, LPCSTR, LPCSTR, UINT, WORD);

static void MsgBoxGUI(const char *format, ...) {
    setlocale(LC_ALL, ".UTF-8");

    // 1) 사용자 메시지를 먼저 별도 버퍼에 포맷
    char userMsg[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(userMsg, sizeof(userMsg), format, args);
    va_end(args);;

#ifdef OCSTL_OS_WINDOWS
int r = 0;

// 2) MessageBoxExA 포인터 얻기 (컴파일러별)
#ifdef OCSTL_CC_TCC
HMODULE h = LoadLibraryA("user32.dll");
PFN_MessageBoxExA pMessageBoxExA = h
                                       ? (PFN_MessageBoxExA) GetProcAddress(h, "MessageBoxExA")
                                       : NULL;
#else
HMODULE h = NULL;
PFN_MessageBoxExA pMessageBoxExA = MessageBoxExA;
#endif

// 3) 컴파일러 무관하게 동일한 템플릿으로 본문 빌드
char path[MAX_PATH];
GetModuleFileNameA(NULL, path, sizeof(path));

char buf[2048]; // path(260) + userMsg(1024) + 템플릿 여유
snprintf(buf, sizeof(buf),
             "Runtime Error!\n\n"
             "Program: %s\n\n"
             "MsgBoxGUI() has been called\n\n"
             "%s\n\n"
             "(Press Retry to debug the application)",
         path, userMsg);

// 4) 다이얼로그 표시
    if (pMessageBoxExA) {
    r = pMessageBoxExA(
        NULL,
        buf,
        _MsgTitle(),
        MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_DEFBUTTON3 | MB_TASKMODAL,
        0
    );
}

// 5) 정리는 반드시 MessageBox 호출 후에
    if (h) FreeLibrary(h);

    switch (r) {
case IDABORT: ExitProcess(3);
    break; // 중단
case IDRETRY: DebugBreak();
    break; // 다시 시도 → 디버거 어태치
case IDIGNORE: break; // 무시
}
#endif
}
#elif defined(OCSTL_OS_LINUX)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
struct lang_labels {
    const char *prefix; // LANG의 앞 2글자
    const char *abort_s;
    const char *retry_s;
    const char *ignore_s;
    const char *title_s; // "Runtime Library" 번역
};

static const struct lang_labels kLangs[] = {
    {"ko", "중단", "다시 시도", "무시", "런타임 라이브러리"},
    {"ja", "中止", "再試行", "無視", "ランタイム ライブラリ"},
    {"zh", "中止", "重试", "忽略", "运行时库"},
    {"de", "Abbrechen", "Wiederholen", "Ignorieren", "Laufzeitbibliothek"},
    {"fr", "Abandonner", "Réessayer", "Ignorer", "Bibliothèque d'exécution"},
    {"es", "Anular", "Reintentar", "Omitir", "Biblioteca de ejecución"},
    {"ru", "Прервать", "Повторить", "Пропустить", "Библиотека времени выполнения"},
    {"it", "Interrompi", "Riprova", "Ignora", "Libreria runtime"},
    {"pt", "Anular", "Repetir", "Ignorar", "Biblioteca de runtime"},
    {"en", "Abort", "Retry", "Ignore", "Runtime Library"},
    {NULL, NULL, NULL, NULL, NULL}
};

static const struct lang_labels *detect_lang(void) {
    const char *lang = getenv("LC_ALL");
    if (!lang || !*lang) lang = getenv("LC_MESSAGES");
    if (!lang || !*lang) lang = getenv("LANG");
    if (!lang || !*lang) lang = "en";

    // "ko_KR.UTF-8" -> "ko" 비교
    for (int i = 0; kLangs[i].prefix; i++) {
        size_type64 n = strlen(kLangs[i].prefix);
        if (strncmp(lang, kLangs[i].prefix, n) == 0 &&
            (lang[n] == '\0' || lang[n] == '_' || lang[n] == '.')) {
            return &kLangs[i];
        }
    }
    // 마지막 엔트리(en) fallback
    int last = 0;
    while (kLangs[last + 1].prefix) last++;
    return &kLangs[last];
}

// MessageBoxExA 반환값과 동일한 상수 (이식성 위해)
#define IDABORT   3
#define IDRETRY   4
#define IDIGNORE  5

static void get_exe_path(char *out, size_type64 n) {
    ssize_t r = readlink("/proc/self/exe", out, n - 1);
    if (r < 0) r = 0;
    out[r] = '\0';
}

// Windows의 MessageBoxExA(MB_ABORTRETRYIGNORE|MB_ICONERROR) 대응
static int zenity_abort_retry_ignore(const char *title, const char *body) {
    int pipefd[2];
    if (pipe(pipefd) < 0) return IDABORT;

    pid_t pid = fork();
    if (pid < 0) {
        close(pipefd[0]);
        close(pipefd[1]);
        return IDABORT;
    }
    const struct lang_labels *L = detect_lang();
    if (pid == 0) {
        // 자식: zenity 실행
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);


        char ok_opt[128], cancel_opt[128], extra_opt[128];
        snprintf(ok_opt, sizeof(ok_opt), "--ok-label=%s", L->ignore_s);
        snprintf(cancel_opt, sizeof(cancel_opt), "--cancel-label=%s", L->abort_s);
        snprintf(extra_opt, sizeof(extra_opt), "--extra-button=%s", L->retry_s);

        // zenity(GTK/Mesa)의 경고 메시지 버리기
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull >= 0) {
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }

        execlp("zenity", "zenity",
               "--question",
               "--title", L->title_s, // title 인자도 로케일 반영하려면 이걸 사용
               "--text", body,
               "--icon=dialog-error",
               ok_opt,
               cancel_opt,
               extra_opt,
               "--default-cancel",
               "--width=420",
               (char *) NULL);
        _exit(127);
    }

    // 부모: 결과 수집
    close(pipefd[1]);
    char out[256] = {0};
    ssize_t n = read(pipefd[0], out, sizeof(out) - 1);
    close(pipefd[0]);
    if (n > 0) out[n] = '\0';

    int status = 0;
    waitpid(pid, &status, 0);
    int code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;

    // zenity 종료 코드:
    //   0  -> --ok-label (무시)          → IDIGNORE
    //   1  -> --cancel-label 또는 extra-button
    //         (extra-button을 눌렀다면 stdout에 라벨 문자열이 출력됨)
    //   5  -> 타임아웃
    if (code == 0) {
        return IDIGNORE; // --ok-label 클릭
    }
    size_type64 Llen = strlen(out);
    while (Llen > 0 && (out[Llen - 1] == '\n' || out[Llen - 1] == '\r')) out[--Llen] = '\0';

    if (strcmp(out, L->retry_s) == 0) return IDRETRY;
    return IDABORT; // 취소 버튼, 창 닫기(X), Esc 모두 여기로
}

static void MsgBoxGUI(const char *format, ...) {
    char userMsg[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(userMsg, sizeof(userMsg), format, args);
    va_end(args);

    char path[PATH_MAX];
    get_exe_path(path, sizeof(path));

    char buf[1024];
    snprintf(buf, sizeof(buf),
             "<b>Runtime Error!</b>\n\n"
             "Program: %s\n\n"
             "MsgBoxGUI() has been called\n\n"
             "%s\n\n"
             "(Press Retry to debug the application)",
             path, userMsg);

    int r = zenity_abort_retry_ignore("Runtime Library", buf);

    switch (r) {
        case IDABORT: fprintf(stderr, "abort\n");
            _exit(3);
        case IDRETRY: fprintf(stderr, "retry\n");
#if defined(OCSTL_CC_TCC)
            raise(SIGTRAP); // = DebugBreak (TCC: no __builtin_trap)
#else
            __builtin_trap(); // = DebugBreak
#endif
            break;
        case IDIGNORE: fprintf(stderr, "ignore\n");
            break;
    }
}

#elif defined(OASTL_OS_MACOD)


#endif
#endif //OPENCSTL_MSG_H
