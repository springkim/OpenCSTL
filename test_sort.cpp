#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <chrono>
#include <execution>
typedef unsigned long long uint64;
static uint64 _seed64 = 1;

static void srand64(uint64 seed) {
    _seed64 = seed;
}

static uint64 rand64(void) {
    _seed64 = (_seed64 * 6364136223846793005ULL) + 1442695040888963407ULL;
    return (uint64) _seed64;
}

int main() {
    srand64(77);
    uint64 N = 100000000;
    std::vector<uint64> v;
    for (uint64 i = 0; i < N; i++) {
        v.push_back(rand64());
    }
    std::chrono::system_clock::time_point t_beg = std::chrono::system_clock::now();


    std::sort(v.begin(), v.end());

    std::chrono::system_clock::time_point t_end = std::chrono::system_clock::now();

    std::chrono::duration<double, std::milli> ms = t_end - t_beg;
    std::cout << "C++: " << ms.count() << " ms" << std::endl;
}
