#include "opencstl.h"

int main() {
    BITSET b = new_bitset(50);

    bitset.set_at(b, 10,true);
    bitset.set_at(b, 0,true);
    int cnt = bitset.count_bits(b);
    printf("%d\n", cnt);

    char *str = bitset.to_string(b);
    puts(str);

    bitset.free(b);
    return 0;
}