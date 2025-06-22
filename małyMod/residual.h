#ifndef RESIDUAL_H
#define RESIDUAL_H

#include <unordered_map>
#include <utility>
#include <vector>
#include <limits>
#include <queue>
#include <map>

#include "prz_wiel.h"

struct Edge {
    int from, to; //skad, dokad
    int capacity, flow;
    int repairCost;  // koszt naprawy tej krawędzi
    Edge* reverse;

    // Konstruktor z parametrem kosztu naprawy
    Edge(int from, int to, int capacity, int repairCost = 0, int flow = 0)
        : from(from), to(to), capacity(capacity), flow(flow), repairCost(repairCost), reverse(nullptr) {
    }
    //operator do porównywania krawędzi i oceniania czy sa takie same (na podstawie id wierzchołków, zaklada brak )
    const bool operator==(const Edge& other) {
        return (this->from == other.from && this->to == other.to);
    }
};

class ResidualNetwork {
public:
    ResidualNetwork(int vertices);
    ~ResidualNetwork();
    // Stara wersja addEdge bez kosztu (just in case)
    void addEdge(int from, int to, int capacity);

    // Nowa wersja addEdge z kosztem naprawy
    void addEdge(int from, int to, int capacity, int repairCost);

    // podstawowa Werjsa
    int maxFlow(int source, int sink);

    // Funkcja minimalnego kosztu maksymalnego przepływu
    std::pair<int, int> minCostMaxFlow(int source, int sink);

    std::vector<std::vector<Edge*>> adjList;

    std::vector<Edge> saveEdgesWithFlow(); // Zwraca krawędzie z przepływem w grafie
    static int compareAndSumUniqueEdgeCosts(
        const std::vector<Edge>& flowSet1,
        const std::vector<Edge>& flowSet2);

    void setCoordinates(int vertex, Point p);
    void printCoordinates();

    void printPath();

    //wierzcholek -> int x, int y
    std::unordered_map<int, Point> coordinates;

private:
    int vertices;

    
    


    bool bfs(int source, int sink, std::vector<Edge*>& parent);

    // Bellman-Ford do znajdowania najtańszej ścieżki powiększającej
    bool bellmanFord(int source, int sink, std::vector<Edge*>& parent);
};

#endif // RESIDUAL_H
