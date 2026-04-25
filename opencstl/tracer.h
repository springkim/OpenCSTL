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

#ifndef OPENCSTL_TRACER_H
#define OPENCSTL_TRACER_H
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "logging.h"
#include "salloc.h"
#include "galloc.h"
//#include "van_emde_boas_tree.h"
#define _1MB (1024*1024)
#define _512KB (1024*512)

#ifdef OPENCSTL_TRACER
typedef struct ZMem {
    void *ptr;
    char *file;
    char *func;
    int line;
} ZMEM;

ZMEM zmem[8192] = {0};
//void *zalloc_vector[_512KB] = {0};
size_type64 zalloc_size = 0;
size_type64 zalloc_count = 0;

static void zinsert(void *ptr, char *file, const char *func, int line) {
    zmem[zalloc_size].ptr = ptr;
    zmem[zalloc_size].file = file;
    zmem[zalloc_size].func = (char*)func;
    zmem[zalloc_size].line = line;
    zalloc_size++;
    zalloc_count++;
}

static void zremove(void *ptr) {
    for (size_type64 i = 0; i < zalloc_size; i++) {
        if (zmem[i].ptr == ptr) {
            memmove(zmem + i, zmem + i + 1, (zalloc_size - i - 1) * sizeof(ZMEM));
            zalloc_size--;
            zmem[zalloc_size].ptr = NULL;
            zmem[zalloc_size].file = NULL;
            zmem[zalloc_size].func = NULL;
            zmem[zalloc_size].line = 0;
            return;
        }
    }
}

// static void zappend(void *ptr) {
//     zalloc_vector[zalloc_size++] = ptr;
//     zalloc_count++;
// }
//
// static void zerase(void *ptr) {
//     for (size_type64 i = 0; i < zalloc_size; i++) {
//         if (zalloc_vector[i] == ptr) {
//             memcpy(zalloc_vector + i, zalloc_vector + i + 1, (zalloc_size - i - 1) * sizeof(void *));
//             zalloc_size--;
//             zalloc_vector[zalloc_size] = NULL;
//             return;
//         }
//     }
// }
#endif
#ifdef OPENCSTL_TRACER
static void opencstl_exit(void) {
    if (zalloc_size > 0) {
        logging.warning("%d memory blocks were not released", zalloc_size);

        for (size_type64 i = 0; i < zalloc_size; i++) {
            logging.debug("%p: %s, %s, %d", zmem[i].ptr, zmem[i].file, zmem[i].func, zmem[i].line);
        }
    }
    logging.debug("OpenCSTL tracer end");
    logging.debug("zalloc count: %d", zalloc_count);
}
#endif
#ifdef OPENCSTL_TRACER

#if defined(__GNUC__) || defined(__clang__)
__attribute__((constructor))
#endif
static int opencstl_init(void) {
    //size_type64 SZ = _512KB;
    //zalloc_vector = salloc(SZ);
    //memset(zalloc_vector, 0, SZ);

    logging.debug("OpenCSTL tracer start");
    //htm = htm_new();

    //mt19937.seed(time(NULL));
    atexit(opencstl_exit);
    return 0;
}

#if defined(_MSC_VER)
# pragma section(".CRT$XCU",read)
__declspec(allocate(".CRT$XCU")) static int (*__p)(void) = opencstl_init;
# pragma data_seg()
#endif
#endif
#endif
