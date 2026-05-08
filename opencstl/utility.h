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
#pragma once
#ifndef HG_6851F11BA5132E124F4F2633B594CC2AE6340950A32E371CA41C0A3D9A476F60_H
#define HG_6851F11BA5132E124F4F2633B594CC2AE6340950A32E371CA41C0A3D9A476F60_H
// floor(sqrt(x)) — Integer-only, no floating-point or division operations
// Processes 2 bits at a time → Up to 16 loops for a 32-bit input
static unsigned int fast_sqrt(unsigned int x) {
    unsigned int res = 0;
    unsigned int bit = 1u << 30; // 32bit 내 최대 4의 거듭제곱
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

//   ratio = √2 + (√9 - √2) · K / (K + √capacity)
//   capacity →   0 : ratio → √9 = 3.0
//   capacity → ∞   : ratio → √2 ≈ 1.414
//   K = conversion rate. If K = 16, then √capacity = K points (i.e., capacity 256) is exactly halfway.
static int get_new_capacity(int _capacity) {
    if (_capacity < 2) return 2;

    enum {
        SQRT2_Q16 = 92682, // √2 · 65536
        DIFF_Q16 = 103926, // (√9 - √2) · 65536
        K = 16
    };

    unsigned int s = fast_sqrt((unsigned int) _capacity);
    int ratio_q16 = SQRT2_Q16 + (int) ((unsigned int) DIFF_Q16 * K / (K + s));

    long long grown = ((long long) _capacity * ratio_q16) >> 16;
    if (grown <= _capacity) grown = _capacity + 1;
    if (grown > 2147483647LL) grown = 2147483647LL;
    return (int) grown;
}

// #include <math.h>
// #include <float.h>
// static int float_cmp(const void* a, const void* b) {
//     const float* pa = (const float*) a;
//     const float* pb = (const float*) b;
//     float x = *pa, y = *pb;
//     float diff = fabsf(x - y);
//     float scale = fmaxf(fabsf(x), fabsf(y));
//     if (diff <= FLT_EPSILON * fmaxf(1.0f, scale)) return 0;
//     return (x < y) ? -1 : 1;
// }
#endif
