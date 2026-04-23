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

#ifndef OPENCSTL_SWAP_H
#define OPENCSTL_SWAP_H
#include <stddef.h>
#include <string.h>

#define SWAP_STACK_BUF_SIZE 256

void swap(void *a, void *b, size_type64 sz) {
    unsigned char stack_buf[SWAP_STACK_BUF_SIZE];
    unsigned char *tmp;
    size_type64 i;

    if (a == NULL || b == NULL || a == b || sz == 0) {
        return;
    }
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
