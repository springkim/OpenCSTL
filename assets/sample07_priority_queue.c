#include "opencstl.h"

typedef struct {
    int cost, to;
} Edge;

int EdgeCMP(const void *a, const void *b) {
    return ((Edge *) a)->cost > ((Edge *) b)->cost ? -1 : ((Edge *) a)->cost < ((Edge *) b)->cost;
}

VECTOR(int) dijkstra(Edge **g, int s) {
    VECTOR(int) d = new_vector(int);
    QUEUE(Edge) pq = new_priority_queue(Edge, EdgeCMP);
    assign(d, size(g), 99999);
    d[s] = 0;
    push(pq, ((Edge){0, s}));
    while (!empty(pq)) {
        Edge e = top(pq);
        pop(pq);
        for (int i = 0; i < size(g[e.to]); i++) {
            Edge *n = &g[e.to][i];
            if (d[n->to] > d[e.to] + n->cost) {
                d[n->to] = d[e.to] + n->cost;
                push(pq, ((Edge){d[n->to], n->to}));
            }
        }
    }
    destroy(pq);
    return d;
}

int main() {
    Edge **g = new_vector(Edge*);
    assign(g, 7);
    for (int i = 0; i < size(g); i++) {
        g[i] = new_vector(Edge);
    }
    push_back(g[1], ((Edge){10, 2}));
    push_back(g[1], ((Edge){30, 3}));
    push_back(g[1], ((Edge){15, 4}));
    push_back(g[2], ((Edge){20, 5}));
    push_back(g[3], ((Edge){ 5, 6}));
    push_back(g[4], ((Edge){ 5, 3}));
    push_back(g[4], ((Edge){20, 6}));
    push_back(g[5], ((Edge){20, 6}));
    push_back(g[6], ((Edge){20, 4}));

    VECTOR(int) d = dijkstra(g, 1);
    for (int i = 0; i < size(d); i++) {
        printf("%d\n", d[i]);
    }

    destroy(d);
    for (int i = 0; i < size(g); i++)
        destroy(g[i]);
    destroy(g);
}
