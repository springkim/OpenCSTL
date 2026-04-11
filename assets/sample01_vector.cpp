#include<iostream>
#include<vector>
#include<algorithm>
int main() {
    std::vector<int> v;
    for (int i = 100 ; i > 0; i--) {
        v.push_back(i);
    }
    std::sort(v.begin(), v.end(),std::less<int>());
    for (auto it = v.begin(); it != v.end(); it++) {
        std::cout << *it << std::endl;
    }
    return 0;
}