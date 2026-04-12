//
// Created by spring on 2026. 4. 12..
//


#ifndef OPENCSTL_PREMAIN_H
#define OPENCSTL_PREMAIN_H
#include <stdio.h>
#include "salloc.h"
#include "logging.h"
void **zalloc_vector = NULL;
size_t zalloc_size = 0;

static void zappend(void *ptr) {
    zalloc_vector[zalloc_size++] = ptr;
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

void opencstl_exit(void) {
    if (zalloc_size > 0) {
        logging.warning("%d memory blocks were not freed", zalloc_size);
    }
    puts("exit");
}

#if defined(__GNUC__)
__attribute__((constructor))
#endif
int opencstl_init(void) {
    size_t SZ = 1024 * 1024;
    zalloc_vector = salloc(SZ); //1MB
    memset(zalloc_vector, 0, SZ);
    puts("premain");

    atexit(opencstl_exit);
    return 0;
}
#if defined(_MSC_VER)
# pragma section(".CRT$XCU",read)
__declspec(allocate(".CRT$XCU"))static int (*p)(void)= opencstl_init;
# pragma data_seg()
#endif


#endif //OPENCSTL_PREMAIN_H
