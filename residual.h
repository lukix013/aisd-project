#ifndef RESIDUAL_H
#define RESIDUAL_H

#include <unordered_map>
#include <utility>
#include <vector>
#include <limits>
#include <queue>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>

#include "prz_wiel.h"

struct Edge {
    int from;          //wierzchołek początkowy krawędzi
    int to;            // wierzchołek końcowy krawędzi
    int capacity;      // przepustowość krawędzi
    int flow;          //aktualny przepływ przez krawędź
    int repairCost;    // koszt naprawy (używany jako koszt przepływu)
    Edge* reverse;     //wskaźnik do krawędzi rewersyjnej w grafie

    Edge(int from, int to, int capacity, int repairCost = 0, int flow = 0)
        : from(from), to(to), capacity(capacity), flow(flow), repairCost(repairCost), reverse(nullptr) {
    }
};

class ResidualNetwork {
public:
    ResidualNetwork(int vertices);
    ~ResidualNetwork();
    void addEdge(int from, int to, int capacity);
    void addEdge(int from, int to, int capacity, int repairCost);
    int maxFlow(int source, int sink);
    std::pair<int, int> minCostMaxFlow(int source, int sink);

    std::vector<std::vector<Edge*>> adjList; // Lista sąsiedztwa przechowująca wskaźniki do krawędzi.
    void setCoordinates(int vertex, Point p);

    void printCoordinates();

    std::vector<Edge> saveEdgesWithFlow(); // Zwraca krawędzie z przepływem w grafie
    static int compareAndSumUniqueEdgeCosts(
        const std::vector<Edge>& flowSet1,
        const std::vector<Edge>& flowSet2);
    void printPath();
    void printPathToFile(std::ofstream& output);

    std::unordered_map<int, Point> coordinates; //Mapa wierzchołków do ich współrzędnych.

private:
    int vertices; //Liczba wierzchołków w grafie.

    bool bfs(int source, int sink, std::vector<Edge*>& parent);
    bool bellmanFord(int source, int sink, std::vector<Edge*>& parent);
    bool findNegativeCycle(std::vector<Edge*> &cycle);
    void cancelNegativeCycles();
};

#endif // RESIDUAL_H
