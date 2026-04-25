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
//               Copyright (C) 2018-2026, Kim Bomm, all rights reserved.
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

#ifndef OPENCSTL_VERIFY_H
#define OPENCSTL_VERIFY_H
#include <assert.h>
#include "logging.h"

#define verify(EXPR) do { if(!(EXPR)) __verify(#EXPR,__FILE__,__LINE__); } while(0)

static int __verify(char *expression, char *file, int line) {
    logging.error("Verification failed: %s, file %s, line %d",
                  expression, file, line);
    fflush(stdout);
    fflush(stderr);
    abort();
}

#define fault(STR) do{_fault((STR),__FILE__,__LINE__);}while(0)

static void _fault(char *str, char *file, int line) {
    logging.error("Fault: %s, file %s, line %d", str, file, line);
    fflush(stdout);
    fflush(stderr);
    exit(-1);
}
#endif //OPENCSTL_VERIFY_H
