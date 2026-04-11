#pragma once
#if !defined(_OPENCSTL_LOGGING_H)
#define _OPENCSTL_LOGGING_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "crossplatform.h"

#ifdef OCSTL_OS_WINDOWS
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

const char *__red = "\033[31m";
const char *__green = "\033[32m";
const char *__yellow = "\033[33m";
const char *__blue = "\033[34m";
const char *__magenta = "\033[35m";
const char *__cyan = "\033[36m";
const char *__white = "\033[37m";
const char *__reset = "\033[0m";

#ifdef OCSTL_OS_WINDOWS

static int s_ansi_enabled = -1;

static int try_enable_ansi(void) {
    if (s_ansi_enabled == -1) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (!GetConsoleMode(h, &mode)) {
            s_ansi_enabled = 0;
        } else {
            s_ansi_enabled = SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) ? 1 : 0;
        }
    }
    return s_ansi_enabled;
}

static WORD ansi_to_winattr(const char *color) {
    if (color == __red) return FOREGROUND_RED | FOREGROUND_INTENSITY;
    if (color == __green) return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    if (color == __yellow) return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    if (color == __blue) return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    if (color == __magenta) return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    if (color == __cyan) return FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    if (color == __white) return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
}

#endif // OCSTL_OS_WINDOWS

static int __vcprintln(const char *color, const char *format, va_list args) {
    int ret;

#ifdef OCSTL_OS_WINDOWS
    if (try_enable_ansi()) {
        printf("%s", color);
        ret = vprintf(format, args);
        printf("%s\n", __reset);
    } else {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h, ansi_to_winattr(color));
        ret = vprintf(format, args);
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("\n");
    }
#elif defined(OCSTL_OS_LINUX) || defined(OCSTL_OS_MACOS)
    printf("%s", color);
    ret = vprintf(format, args);
    printf("%s\n", __reset);
#else
    ret = vprintf(format, args);
    printf("\n");
#endif

    return ret;
}

static char *__vcsprintf(const char *format, ...) {
    static char buf[1024] = {0};
    va_list args;
    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);
    return buf;
}

#if defined(OCSTL_OS_MACOS)

void __fatal_message_box(const char *msg) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd),
             "osascript -e 'display dialog \"%s\" with title \"FATAL\" with icon stop buttons {\"OK\"} default button \"OK\"' > /dev/null 2>&1",
             msg);
    system(cmd);
}
#endif

int __clogging(const char *color, const char *format, ...) {
    va_list args;
    int ret;
    va_start(args, format);
    ret = __vcprintln(color, format, args);
    va_end(args);
    return ret;
}

static int _logging_error(const char *format, ...) {
    va_list args;
    int ret;
    va_start(args, format);
    ret = __vcprintln(__red, format, args);
    va_end(args);
    return ret;
}

static int _logging_debug(const char *format, ...) {
#if defined(OCSTL_DEBUG)
    va_list args;
    int ret;
    va_start(args, format);
    ret = __vcprintln(__green, format, args);
    va_end(args);
    return ret;
#else
    return 0;
#endif
}

static int _logging_warning(const char *format, ...) {
    va_list args;
    int ret;
    va_start(args, format);
    ret = __vcprintln(__yellow, format, args);
    va_end(args);
    return ret;
}

static int _logging_info(const char *format, ...) {
    va_list args;
    int ret;
    va_start(args, format);
    ret = __vcprintln(__blue, format, args);
    va_end(args);
    return ret;
}

static int _logging_critical(const char *format, ...) {
    va_list args;
    int ret;
    va_start(args, format);
    ret = __vcprintln(__magenta, format, args);
    va_end(args);
    return ret;
}

static int _logging_fatal(const char *format, ...) {
    va_list args;
    char *ret;
    va_start(args, format);
    ret = __vcsprintf(format, args);
    va_end(args);
    __fatal_message_box(ret);
    exit(EXIT_FAILURE);
    return -1;
}

static int _logging_message(const char *format, ...) {
    va_list args;
    int ret;
    va_start(args, format);
    ret = __vcprintln(__white, format, args);
    va_end(args);
    return ret;
}

typedef int (*logging_fn)(const char *format, ...);

typedef struct LOGGING {
    logging_fn debug;
    logging_fn info;
    logging_fn warning;
    logging_fn error;
    logging_fn critical;
    logging_fn fatal;

    logging_fn message;
} LOGGING;

static LOGGING logging = {
    _logging_debug,
    _logging_info,
    _logging_warning,
    _logging_error,
    _logging_critical,

    _logging_fatal,

    _logging_message
};

#endif
