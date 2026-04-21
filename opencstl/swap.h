//
// Created by spring on 4/21/2026.
//

#ifndef OPENCSTL_SWAP_H
#define OPENCSTL_SWAP_H
#include <stddef.h>
#include <string.h>

#define SWAP_STACK_BUF_SIZE 256

void swap(void *a, void *b, size_t sz) {
    unsigned char stack_buf[SWAP_STACK_BUF_SIZE];
    unsigned char *tmp;
    size_t i;

    if (a == NULL || b == NULL || a == b || sz == 0) {
        return;
    }

    /*
     * 작은 크기는 stack buffer 사용
     * 큰 크기는 byte-wise swap (malloc 없이)
     */
    if (sz <= SWAP_STACK_BUF_SIZE) {
        tmp = stack_buf;

        memcpy(tmp, a, sz);
        memcpy(a, b, sz);
        memcpy(b, tmp, sz);
    } else {
        unsigned char *pa = (unsigned char *) a;
        unsigned char *pb = (unsigned char *) b;

        for (i = 0; i < sz; ++i) {
            unsigned char t = pa[i];
            pa[i] = pb[i];
            pb[i] = t;
        }
    }
}
#endif //OPENCSTL_SWAP_H
