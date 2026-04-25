#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "gmp.h"

// mpz_t → 문자열 출력 헬퍼
static void mpz_print(const char *label, mpz_t n) {
    char *s = mpz_get_str(NULL, 10, n);
    printf("%s%s\n", label, s);
    free(s);
}

static void mpz_factorial(mpz_t result, unsigned long n) {
    mpz_set_ui(result, 1);
    for (unsigned long i = 2; i <= n; i++)
        mpz_mul_ui(result, result, i);
}

static void mpz_fibonacci(mpz_t result, unsigned long n) {
    if (n <= 1) {
        mpz_set_ui(result, n);
        return;
    }
    mpz_t a, b, tmp;
    mpz_init_set_ui(a, 0);
    mpz_init_set_ui(b, 1);
    mpz_init(tmp);

    for (unsigned long i = 2; i <= n; i++) {
        mpz_add(tmp, a, b);
        mpz_set(a, b);
        mpz_set(b, tmp);
    }
    mpz_set(result, b);

    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(tmp);
}

int main() {
    mpz_t a, b, result;

    mpz_init(a);
    mpz_init(b);
    mpz_init(result);

    // ── 기본 산술 ─────────────────────────────────────────────
    mpz_set_str(a, "123456789012345678901234567890", 10);
    mpz_set_str(b, "987654321098765432109876543210", 10);

    mpz_add(result, a, b);
    mpz_print("[add]  a + b = ", result);

    mpz_mul(result, a, b);
    mpz_print("[mul]  a * b = ", result);

    mpz_tdiv_q(result, b, a);
    mpz_print("[div]  b / a = ", result);

    mpz_mod(result, b, a);
    mpz_print("[mod]  b % a = ", result);

    // ── 거듭제곱 ──────────────────────────────────────────────
    mpz_set_ui(a, 2);
    mpz_pow_ui(result, a, 128);
    mpz_print("[pow]  2^128 = ", result);

    // ── 팩토리얼 ──────────────────────────────────────────────
    mpz_factorial(result, 50);
    mpz_print("[fact] 50!   = ", result);

    // ── 피보나치 ──────────────────────────────────────────────
    mpz_fibonacci(result, 100);
    mpz_print("[fib]  F100  = ", result);

    // ── GCD / LCM ─────────────────────────────────────────────
    mpz_set_str(a, "123456789012345678901234567890", 10);
    mpz_set_str(b, "987654321098765432109876543210", 10);

    mpz_gcd(result, a, b);
    mpz_print("[gcd]  gcd(a,b) = ", result);

    mpz_lcm(result, a, b);
    mpz_print("[lcm]  lcm(a,b) = ", result);

    // ── 비교 ──────────────────────────────────────────────────
    int cmp = mpz_cmp(a, b);
    printf("[cmp]  a %s b\n", cmp < 0 ? "<" : cmp > 0 ? ">" : "==");

    // ── 비트 연산 ─────────────────────────────────────────────
    mpz_and(result, a, b);
    mpz_print("[and]  a & b = ", result);

    mpz_ior(result, a, b);
    mpz_print("[or]   a | b = ", result);

    mpz_xor(result, a, b);
    mpz_print("[xor]  a ^ b = ", result);

    // ── 소수 판별 ─────────────────────────────────────────────
    mpz_set_str(a, "170141183460469231731687303715884105727", 10);
    int prime = mpz_probab_prime_p(a, 25);
    mpz_print("[prime] M127 = ", a);
    printf("        -> %s\n",
           prime == 2 ? "definitely prime" : prime == 1 ? "probably prime" : "composite");

    // ── 정리 ──────────────────────────────────────────────────
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(result);

    return 0;
}
