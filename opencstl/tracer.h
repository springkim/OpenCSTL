//
// Created by spring on 2026. 4. 12..
//


#ifndef OPENCSTL_TRACER_H
#define OPENCSTL_TRACER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logging.h"
#include "mt19937.h"
//#include "van_emde_boas_tree.h"
#define _1MB (1024*1024)
#define _512KB (1024*512)

#ifdef OPENCSTL_TRACER
void *zalloc_vector[_512KB] = {0};
size_t zalloc_size = 0;
size_t zalloc_count = 0;


static void zappend(void *ptr) {
    zalloc_vector[zalloc_size++] = ptr;
    zalloc_count++;
}

static void zerase(void *ptr) {
    for (size_t i = 0; i < zalloc_size; i++) {
        if (zalloc_vector[i] == ptr) {
            memcpy(zalloc_vector + i, zalloc_vector + i + 1, (zalloc_size - i - 1) * sizeof(void *));
            zalloc_size--;
            zalloc_vector[zalloc_size] = NULL;
            return;
        }
    }
}
#endif
#ifdef OPENCSTL_TRACER
static void opencstl_exit(void) {
    if (zalloc_size > 0) {
        logging.warning("%d memory blocks were not released", zalloc_size);
    }
    logging.debug("opencstl_exit");
    logging.debug("zalloc_count: %d", zalloc_count);
}
#endif
#ifdef OPENCSTL_TRACER
#if defined(__GNUC__) || defined(__clang__)
__attribute__((constructor))
#endif
static int opencstl_init(void) {
    //size_t SZ = _512KB;
    //zalloc_vector = salloc(SZ);
    //memset(zalloc_vector, 0, SZ);

    logging.info("opencstl_init");

    //htm = htm_new();

    random.seed(time(NULL));
    atexit(opencstl_exit);
    return 0;
}

#if defined(_MSC_VER)
# pragma section(".CRT$XCU",read)
__declspec(allocate(".CRT$XCU")) static int (*__p)(void)= opencstl_init;
# pragma data_seg()
#endif
#endif
#endif
