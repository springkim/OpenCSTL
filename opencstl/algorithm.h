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
#pragma once
#if !defined(_OPENCSTL_C_ALGORITHM_H)
#define _OPENCSTL_C_ALGORITHM_H


#include "defines.h"
#include "compare.h"
#include "iterator.h"
#include "van_emde_boas_tree.h"

bool is_even(void *it) {
    return true;
}

#define fill(_First, _Last, _Value) _cstl_fill(_First, _Last, &(__typeof__(_Value)){_Value}, sizeof(_Value))
OPENCSTL_FUNC void _cstl_fill(void *_begin, void *_end, void *_value, size_t _value_size) {
    void *it = _begin;
    while (it != _end) {
        memcpy(it, _value, _value_size);
        it = cstl_next(it);
    }
}

// ███╗░░░███╗██╗███╗░░██╗░░░░░░░░░░░██╗░░░░░░░███╗░░░███╗░█████╗░██╗░░██╗
// ████╗░████║██║████╗░██║░░░░░░░░░░██╔╝░░░░░░░████╗░████║██╔══██╗╚██╗██╔╝
// ██╔████╔██║██║██╔██╗██║░░░░░░░░░██╔╝░░░░░░░░██╔████╔██║███████║░╚███╔╝░
// ██║╚██╔╝██║██║██║╚████║░░░░░░░░██╔╝░░░░░░░░░██║╚██╔╝██║██╔══██║░██╔██╗░
// ██║░╚═╝░██║██║██║░╚███║░░░░░░░██╔╝░░░░░░░░░░██║░╚═╝░██║██║░░██║██╔╝╚██╗
// ╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝░░░░░░░╚═╝░░░░░░░░░░░╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝░░╚═╝

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif


#define max_element(...) _MAX_ELEMENT_DISPATCH(__VA_ARGS__, NULL)
#define _MAX_ELEMENT_DISPATCH(_begin, _end, _cmp, ...) _cstl_max_element(_begin, _end, (_OpenCSTLCompareFunc)(_cmp))

OPENCSTL_FUNC void *_cstl_max_element(void *_begin, void *_end, _OpenCSTLCompareFunc _cmp) {
    if (_begin == _end) {
        return _end;
    }

    if (_cmp == NULL) {
        Interval *tm = iveb_find(iveb, _begin);
        _cmp = LESS(tm->type_name);
        if (_cmp == NULL) {
            _cmp = _memcmp_funcs[tm->type_size];
        }
        verify(_cmp != NULL);
    }

    void *max_it = _begin;
    void *it = cstl_next(_begin);
    while (it != _end) {
        if (_cmp(it, max_it) > 0) {
            max_it = it;
        }
        it = cstl_next(it);
    }
    return max_it;
}

#define min_element(...) _MIN_ELEMENT_DISPATCH(__VA_ARGS__, NULL)
#define _MIN_ELEMENT_DISPATCH(_begin, _end, _cmp, ...) _cstl_min_element(_begin, _end, (_OpenCSTLCompareFunc)(_cmp))

OPENCSTL_FUNC void *_cstl_min_element(void *_begin, void *_end, _OpenCSTLCompareFunc _cmp) {
    if (_begin == _end) {
        return _end;
    }

    if (_cmp == NULL) {
        Interval *tm = iveb_find(iveb, _begin);
        _cmp = LESS(tm->type_name);
        if (_cmp == NULL) {
            _cmp = _memcmp_funcs[tm->type_size];
        }
        verify(_cmp != NULL);
    }

    void *min_it = _begin;
    void *it = cstl_next(_begin);
    while (it != _end) {
        if (_cmp(it, min_it) < 0) {
            min_it = it;
        }
        it = cstl_next(it);
    }
    return min_it;
}

#endif //_OPENCSTL_C_ALGORITHM_H
