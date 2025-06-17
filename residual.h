#ifndef RESIDUAL_H
#define RESIDUAL_H

#include <unordered_map>
#include <utility>
#include <vector>
#include <limits>
#include <queue>

#include "prz_wiel.h"

struct Edge {
    int from, to; //skad, dokad
    int capacity, flow;
    int repairCost;  // koszt naprawy tej krawędzi
    Edge* reverse;

    // Konstruktor z parametrem kosztu naprawy
    Edge(int from, int to, int capacity, int repairCost = 0)
        : from(from), to(to), capacity(capacity), flow(0), repairCost(repairCost), reverse(nullptr) {}
};

class ResidualNetwork {
public:
    ResidualNetwork(int vertices);

    // Stara wersja addEdge bez kosztu (just in case)
    void addEdge(int from, int to, int capacity);

    // Nowa wersja addEdge z kosztem naprawy
    void addEdge(int from, int to, int capacity, int repairCost);

    // podstawowa Werjsa
    int maxFlow(int source, int sink);

    // Funkcja minimalnego kosztu maksymalnego przepływu
    std::pair<int, int> minCostMaxFlow(int source, int sink);

    std::vector<std::vector<Edge*>> adjList;

    void setCoordinates(int vertex, Point p);
    void printCoordinates();

    //wierzcholek -> int x, int y
    std::unordered_map<int, Point> coordinates;

private:
    int vertices;

    
    


    bool bfs(int source, int sink, std::vector<Edge*>& parent);

    // Bellman-Ford do znajdowania najtańszej ścieżki powiększającej
    bool bellmanFord(int source, int sink, std::vector<Edge*>& parent);
};

#endif // RESIDUAL_H
