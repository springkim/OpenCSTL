#include<iostream>
#include<stack>
#include<queue>
int main() {
    std::stack<int> s;
    std::queue<int> q;
    
    for (int i = 0; i < 10; i++) {
        s.push(i);
        q.push(i);
    }
    
    for (int i = 0; i < 10; i++) {
        std::cout << s.top() << std::endl;
        s.pop();
    }
    for (int i = 0; i < 10; i++) {
        std::cout << q.front() << std::endl;
        q.pop();
    }
}