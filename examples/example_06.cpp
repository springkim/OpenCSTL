#include<queue>
#include<cstdio>

int main() {
    std::priority_queue<int> q;
    for (int i = 0; i < 10; i++) {
        q.push(i);
    }
    while (!q.empty()) {
        printf("[%3d]",q.top());
        q.pop();
    }
    puts("");
    return 0;
}
