#ifndef RESIDUAL_H
#define RESIDUAL_H

#include "zmodyfikujdane.h"
#include <vector>
#include <limits>
#include <queue>



class ResidualNetwork {
public:
    struct Edge {
        int from, to;
        float capacity, flow;
        Edge* reverse;

        Edge(int from, int to, float capacity)
            : from(from), to(to), capacity(capacity), flow(0), reverse(nullptr) {}
    };

    ResidualNetwork(int vertices);
    void addEdge(int from, int to, float capacity);
    void addEdges(std::string& filePath, int mode);
    float maxFlow(int source, int sink);

private:
    int vertices;
    std::vector<std::vector<Edge*>> adjList;

    bool bfs(int source, int sink, std::vector<Edge*>& parent);
};

#endif
