//
// Created by spring on 2026. 4. 9..
//

#pragma once
#if !defined(_OPENCSTL_C_ALGORITHM_H)
#include "defines.h"
#define _OPENCSTL_C_ALGORITHM_H
#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

OPENCSTL_FUNC void fill(void *container, ...) {
    va_list vl;
    void *va_ptr = NULL;
    __cstl_va_start(vl, container, va_ptr);
#if CSTL_USE_VAARG
    void *param1 = __cstl_va_arg_next(vl);
#else
    void *param1 = __cstl_va_arg(va_ptr);
#endif
    

    __cstl_va_end(vl);
}


#endif //_OPENCSTL_C_ALGORITHM_H
