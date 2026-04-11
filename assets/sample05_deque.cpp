#include <iostream>
#include <deque>

int main() {
    std::deque<int> dq;
    for (int i = 0; i < 10; i++) {
        dq.push_back(i);
    }
    std::cout << "deque size: " << dq.size() << std::endl;
    for (int i = 0; i < 10; i++) {
        dq.push_front(i);
    }
    std::cout << "deque size: " << dq.size() << std::endl;
    for (int i = 0; i < dq.size(); i++) {
        std::cout << "[" << dq[i] << "]";
    }
    std::cout << std::endl;
    std::cout << "front : " << dq.front() << std::endl;
    std::cout << "back : " << dq.back() << std::endl;

    dq.assign(5,0);
    // [0] [0] [0] [0] [0] 
    std::cout << "size: " << dq.size() << std::endl;
    for (int i = 0; i < dq.size(); i++) {
        std::cout << "[" << dq[i] << "]";
    }
    std::cout << std::endl;
    
    dq.assign(5, 7);
    dq.resize(10);
    // [7] [7] [7] [7] [7] [0] [0] [0] [0] [0] 
    for (int i = 0; i < dq.size(); i++) {
        std::cout << "[" << dq[i] << "]";
    }
    std::cout << std::endl;
    return 0;
}