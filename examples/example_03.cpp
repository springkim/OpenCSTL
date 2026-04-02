#include <iostream>
#include <list>

int main() {
    std::list<int> lst;
    for (int i = 0; i < 10; i++) {
        int val = 10-i;
        lst.push_back(val);
    }
    
    for (auto it = lst.begin(); it != lst.end(); it++) {
        std::cout << *it << std::endl;
    }
    return 0;
}