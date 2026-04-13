//
// Created by spring on 2026. 4. 9..
//

#pragma once
#if !defined(_OPENCSTL_C_ALGORITHM_H)
#define _OPENCSTL_C_ALGORITHM_H


#include "defines.h"
#include "compare.h"
#include "iterator.h"
#include "van_emde_boas_tree.h"

bool is_even(void *it) {
}

#define fill(_First, _Last, _Value) _cstl_fill(_First, _Last, &(__typeof__(_Value)){_Value}, sizeof(_Value))
OPENCSTL_FUNC void _cstl_fill(void *_begin, void *_end, void *_value, size_t _value_size) {
    void *it = _begin;
    while (it != _end) {
        memcpy(it, _value, _value_size);
        it = cstl_next(it);
    }
}

// ███╗░░░███╗██╗███╗░░██╗░░░░██╗███╗░░░███╗░█████╗░██╗░░██╗
// ████╗░████║██║████╗░██║░░░██╔╝████╗░████║██╔══██╗╚██╗██╔╝
// ██╔████╔██║██║██╔██╗██║░░██╔╝░██╔████╔██║███████║░╚███╔╝░
// ██║╚██╔╝██║██║██║╚████║░██╔╝░░██║╚██╔╝██║██╔══██║░██╔██╗░
// ██║░╚═╝░██║██║██║░╚███║██╔╝░░░██║░╚═╝░██║██║░░██║██╔╝╚██╗
// ╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚═╝░░░░╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝░░╚═╝

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
        if (_cmp == NULL) {
            cstl_error("Compare function is NULL");
            return _end;
        }
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
        if (_cmp == NULL) {
            cstl_error("Compare function is NULL");
            return _end;
        }
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
