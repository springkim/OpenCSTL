#include <iostream>
#include <bitset>

int main() {
    std::bitset<50> b;

    b[10] = true;
    b[0] = true;

    int cnt = b.count();
    std::cout << cnt << std::endl;

    std::string str = b.to_string();
    std::cout << str << std::endl;
    return 0;
}