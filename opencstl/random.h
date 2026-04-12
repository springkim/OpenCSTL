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
#if !defined(_OPENCSTL_C_RANDOM_H)
#define _OPENCSTL_C_RANDOM_H
#include <limits.h>
#define CSTL_RAND64_MAX ULLONG_MAX
#define CSTL_RAND32_MAX UINT_MAX
// static unsigned __int128 _seed64 = 1;
// static unsigned long long _seed32 = 1;

static unsigned long long _seed64 = 1;
static unsigned int _seed32 = 1;

static void cstl_rand_seed32(unsigned int seed) {
    _seed32 = seed;
}

static unsigned int cstl_rand32(void) {
    _seed32 = _seed32 * 1103515245 + 12345;
    return (unsigned int) (_seed32 / 65536);
}

static void cstl_rand_seed64(unsigned long long seed) {
    _seed64 = seed;
}

static unsigned long long cstl_rand64(void) {
    _seed64 = (_seed64 * 6364136223846793005ULL) + 1442695040888963407ULL;
    return (unsigned long long) _seed64;
}

#define rand32              cstl_rand32
#define rand64              cstl_rand64


// shuffle

#endif //_OPENCSTL_C_RANDOM_H
