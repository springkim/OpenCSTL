#include <stdio.h>
#include <gmp.h>

int main(void) {
    mpz_t a, b, c;
    mpz_init(a);                    // 0으로 초기화
    mpz_init_set_ui(b, 12345);      // unsigned long로 초기화
    mpz_init_set_str(c, "999999999999999999999999", 10); // 문자열 + base

    mpz_add(a, b, c);               // a = b + c
    gmp_printf("%Zd\n", a);         // %Zd로 mpz_t 출력

    mpz_clears(a, b, c, NULL);      // 반드시 해제 (NULL 종결)
    return 0;
}