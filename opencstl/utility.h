//
// Created by spring on 4/22/2026.
//

#ifndef OPENCSTL_UTILITY_H
#define OPENCSTL_UTILITY_H
// floor(sqrt(x)) — 정수 전용, FP/division 없음
// 비트 단위로 2비트씩 처리 → 32bit 입력 기준 최대 16회 루프
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

// ratio = √2 + (√9 - √2) · K / (K + √capacity)
//   capacity →   0 : ratio → √9 = 3.0
//   capacity → ∞   : ratio → √2 ≈ 1.414
//   K = 전환 속도. K=16 이면 √capacity = K 지점(=capacity 256)이 딱 중간.
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
#endif //OPENCSTL_UTILITY_H
