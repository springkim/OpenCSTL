#include <iostream>
#include <vector>
#include <queue>

struct Edge {
    int cost, to;
};

struct EdgeCMP {
    bool operator()(const Edge& a, const Edge& b) const {
        return a.cost > b.cost;
    }
};

std::vector<int> dijkstra(std::vector<std::vector<Edge>>& graph, int beg) {
    std::vector<int> d(graph.size(), 99999);
    std::priority_queue<Edge, std::vector<Edge>, EdgeCMP> pqueue;

    d[beg] = 0;
    pqueue.push({0, beg});

    while (!pqueue.empty()) {
        Edge e = pqueue.top();
        pqueue.pop();
        for (const Edge& next : graph[e.to]) {
            if (d[e.to] + next.cost < d[next.to]) {
                d[next.to] = d[e.to] + next.cost;
                pqueue.push({d[next.to], next.to});
            }
        }
    }
    return d;
}

int main() {
    std::vector<std::vector<Edge>> graph(7);

    graph[1].push_back({10, 2});
    graph[1].push_back({30, 3});
    graph[1].push_back({15, 4});
    graph[2].push_back({20, 5});
    graph[3].push_back({ 5, 6});
    graph[4].push_back({ 5, 3});
    graph[4].push_back({20, 6});
    graph[5].push_back({20, 6});
    graph[6].push_back({20, 4});

    std::vector<int> d = dijkstra(graph, 1);

    for (int i = 0; i < (int)d.size(); i++) {
        std::cout << d[i] << "\n";
    }
    return 0;
}