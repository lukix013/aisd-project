#ifndef RESIDUAL_H
#define RESIDUAL_H

#include "zmodyfikujdane.h"
#include <vector>
#include <limits>
#include <queue>
#include <float.h> //max float
#include <map>
#include <algorithm> // std::min



class ResidualNetwork {
public:
    struct Edge {
        int from, to;
        float capacity, flow, cost;
        Edge* reverse;

        Edge(int from, int to, float capacity, float cost)
            : from(from), to(to), capacity(capacity), cost(cost), flow(0), reverse(nullptr) {
        }
    };
    

    ResidualNetwork(int vertices);
    ResidualNetwork(const ResidualNetwork& obj); //moga byc problemy z adjList, i jesli gdzies w grafie brakuje reverse
    ~ResidualNetwork();
    void addEdge(int from, int to, float capacity, float cost);
    void addEdges(std::string& filePath, int mode);
    float maxFlow(int source, int sink);
    ResidualNetwork cheapest(int source, int sink); //wyznacza najtansza droge z maks przeplywem
    void resetFlow();
    void simplePrint();
    bool removeEdge(Edge* e1); //usuwa krawedz i jej reverse z grafu, zwraca true jesli usunieto, false jesli nie byl blad
    ResidualNetwork& operator=(const ResidualNetwork& other);
private:
    std::vector<std::vector<Edge*>> adjList;
    int vertices;
    float mFlow; //maks przeplyw dla tego grafu, trzeba go wyliczyc maxFlow (sam sie ustawi)
    bool bfs(int source, int sink, std::vector<Edge*>& parent);
};

#endif
