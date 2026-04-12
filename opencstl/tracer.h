//
// Created by spring on 2026. 4. 12..
//


#ifndef OPENCSTL_TRACER_H
#define OPENCSTL_TRACER_H
#include <stdio.h>
#include "salloc.h"
#include "logging.h"
//#include "van_emde_boas_tree.h"
#define _1MB (1024*1024)

#ifdef OPENCSTL_TRACER
void **zalloc_vector = NULL;
size_t zalloc_size = 0;
size_t zalloc_count = 0;

void *galloc_vector[_1MB] = {0};


static void zappend(void *ptr) {
    zalloc_vector[zalloc_size++] = ptr;
    zalloc_count++;
}

static void zerase(void *ptr) {
    for (size_t i = 0; i < zalloc_size; i++) {
        if (zalloc_vector[i] == ptr) {
            memcpy(zalloc_vector+i, zalloc_vector+i+1, (zalloc_size-i-1)*sizeof(void*));
            zalloc_size--;
            zalloc_vector[zalloc_size] = NULL;
            return;
        }
    }
}
#endif
void opencstl_exit(void) {
#ifdef OPENCSTL_TRACER
    if (zalloc_size > 0) {
        logging.warning("%d memory blocks were not released", zalloc_size);
    }
    logging.info("opencstl_exit");
    logging.info("zalloc_count: %d", zalloc_count);
#endif
}

#if defined(__GNUC__)
__attribute__((constructor))
#endif
int opencstl_init(void) {
#ifdef OPENCSTL_TRACER
    size_t SZ = _1MB; //1MB
    zalloc_vector = salloc(SZ);
    memset(zalloc_vector, 0, SZ);

    logging.info("opencstl_init");

    //htm = htm_new();
#endif
    atexit(opencstl_exit);
    return 0;
}
#if defined(_MSC_VER)
# pragma section(".CRT$XCU",read)
__declspec(allocate(".CRT$XCU"))static int (*p)(void)= opencstl_init;
# pragma data_seg()
#endif

#endif
