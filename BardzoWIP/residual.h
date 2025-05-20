#ifndef RESIDUAL_H
#define RESIDUAL_H

#include "zmodyfikujdane.h"
#include <vector>
#include <limits>
#include <queue>
#include <float.h> //max float



class ResidualNetwork {
public:
    struct Edge {
        int from, to;
        float capacity, flow, cost;
        Edge* reverse;

        Edge(int from, int to, float capacity, float cost)
            : from(from), to(to), capacity(capacity), cost(cost), flow(0), reverse(nullptr) {}
    };

    ResidualNetwork(int vertices);
    void addEdge(int from, int to, float capacity, float cost);
    void addEdges(std::string& filePath, int mode);
    float maxFlow(int source, int sink);
    std::vector<ResidualNetwork> genWariantyKosztu(); //tworzy wszystkie mozliwe warianty naprawy drog
    ResidualNetwork najbardziejOptymalnaDroga(std::vector<ResidualNetwork> drogi); //wylicza przeplywy i koszty wariantow i zwraca najtaniszy ktory daje najwiekszy Flow

private:
    int vertices;
    float mFlow; //maks przeplyw dla tego grafu, trzeba go wiliczyc maxFlow (sam sie ustawi)
    std::vector<std::vector<Edge*>> adjList;

    bool bfs(int source, int sink, std::vector<Edge*>& parent);
};

#endif
