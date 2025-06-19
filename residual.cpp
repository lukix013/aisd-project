#include "residual.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <queue>  // dodane, bo brakowało
#include "prz_wiel.h"

ResidualNetwork::ResidualNetwork(int vertices) : vertices(vertices) {
    adjList.resize(vertices);
}

void ResidualNetwork::addEdge(int from, int to, int capacity) {
    Edge* forward = new Edge(from, to, capacity);
    Edge* backward = new Edge(to, from, 0);

    forward->reverse = backward;
    backward->reverse = forward;

    adjList[from].push_back(forward);
    adjList[to].push_back(backward);
}

void ResidualNetwork::addEdge(int from, int to, int capacity, int cost) {
    Edge* forward = new Edge(from, to, capacity, cost);
    Edge* backward = new Edge(to, from, 0, -cost);

    forward->reverse = backward;
    backward->reverse = forward;

    adjList[from].push_back(forward);
    adjList[to].push_back(backward);
}

bool ResidualNetwork::bfs(int source, int sink, std::vector<Edge*>& parent) {
    parent.assign(vertices, nullptr); //tablica poprzdników? z jakiego wierzchoka doszliśmy
    std::vector<bool> visited(vertices, false); // wektor czy wierzchołek został odwiedzony
    std::queue<int> q;

    q.push(source);
    visited[source] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (Edge* edge : adjList[current]) {
            if (!visited[edge->to] && edge->capacity > edge->flow) { // sprawdzamy czy odwiedzony i czy pojemnosc wieksza od przepływu
                parent[edge->to] = edge; // ustawiamy że z niego doszliśmy
                visited[edge->to] = true; // odwiedzony true
                q.push(edge->to); 

                if (edge->to == sink)
                    return true; // jak znajdziemy drogę zwracamy true
            }
        }
    }

    return false; // jak brak drogi to false
}

//podstawka

int ResidualNetwork::maxFlow(int source, int sink) {
    int totalFlow = 0;
    std::vector<Edge*> parent(vertices);

    while (bfs(source, sink, parent)) { // działamy tak długo dopóki istnieją drogi powiekszające
        int pathFlow = std::numeric_limits<int>::max(); //ustawiamy dużą wartość

        for (int v = sink; v != source; v = parent[v]->from) {//przechodzenie wstecz w celu znalezienia minimalnej wartości ze ścieżki
            pathFlow = std::min(pathFlow, parent[v]->capacity - parent[v]->flow);
        } // przechodzimy na podstawie tablicy poprzednikow

        for (int v = sink; v != source; v = parent[v]->from) { // aktualizacja przepływu wzdłuż ścieżki powiększającej odejmowanie pathFlow
            Edge* edge = parent[v];
            edge->flow += pathFlow;
            edge->reverse->flow -= pathFlow;
        }

        totalFlow += pathFlow;
    }

    return totalFlow; // gdy brak ścieżek zwracamy
}

//minc cost max flow

bool ResidualNetwork::bellmanFord(int source, int sink, std::vector<Edge*>& parent) {
    std::vector<int> dist(vertices, std::numeric_limits<int>::max());
    parent.assign(vertices, nullptr);
    dist[source] = 0;

    for (int i = 0; i < vertices - 1; i++) { // przechodzimy przez liczba wierzcholkow - 1 chyba że brak kosztu
        
        bool updated = false; // brak aktualizacji kosztu
        for (int u = 0; u < vertices; u++) {
            if (dist[u] == std::numeric_limits<int>::max()) continue;

            for (Edge* e : adjList[u]) { // adjList[0] - wszysteki drogi wychodzące od pierwszego wierzchołka
                if (e->capacity > e->flow && dist[e->to] > dist[u] + e->repairCost) {//jeżeli przepustowość > przepływ i koszto do danego wierzchołka > dystans do startowego + koszt naprawy drogi
                    dist[e->to] = dist[u] + e->repairCost; // zmieniamy koszt dotarcia do wierzchołka
                    parent[e->to] = e; // ustawienie rodzica
                    updated = true; // zaktualizowano więc true
                }
            }
        }
        if (!updated) break; // jeżeli brak aktualizacji przerwij pętle
    }

    return parent[sink] != nullptr; // jeśli znaleziono pętle zwróć true
}

bool ResidualNetwork::findNegativeCycle(std::vector<Edge*> &cycle){
    std::vector<int> dist(vertices, 0);
    std::vector<Edge*> parent(vertices, nullptr);

    int x = -1;

    for (int i = 0; i < vertices; i++) { // robimy to samo co w bellmanFord + dodatkowe przejście, jeśli w dodatkowym przejściu nastąpi zmiana to mamy ujemny cykl
        x = -1;

        for(int u = 0; u < vertices; u++){
            for (Edge* e : adjList[u]) {
                if(e->capacity > e->flow && dist[e->to] > dist[u] + e->repairCost){
                    dist[e->to] = dist[u] + e->repairCost;
                    parent[e->to] = e;
                    x = e->to;
                }
            }
        }
    }

    if (x == -1) return false;

    for (int i = 0; i < vertices; i++) {
        x = parent[x]->from;
    } // cofamy się, x wskazuje na wierzchołek w cyklu ujemnym, cofamy się vertices razy dzięki czemu x znajduje się na cyklu

    cycle.clear();

    int cur = x;
    do {
        cycle.push_back(parent[cur]);
        cur = parent[cur]->from; // rekonstruujemy cykl cofając się po parent
    } while(cur != x);

    std::reverse(cycle.begin(), cycle.end());

    return true;
}

void ResidualNetwork::cancelNegativeCycles(){
    std::vector<Edge*> cycle;

    while (findNegativeCycle(cycle)) {
        int flowToAdd = std::numeric_limits<int>::max();

        for (Edge* e : cycle) {
            flowToAdd = std::min(flowToAdd, e->capacity - e->flow); // maksymalny możliwy przyrost przepływu = min(pojemność - przepływ);
        }

        for (Edge* e : cycle) { 
            e->flow += flowToAdd; // dodajemy przepływ do każdej krawędzi
            e->reverse->flow -= flowToAdd; // zmniejszamy przepływ na krawędziach odwrotnych
        }
    }
}

std::pair<int, int> ResidualNetwork::minCostMaxFlow(int source, int sink) {
    int maxFlow = 0; // makszymalny przepływ na zerto
    int minrepairCost = 0; // minimalny koszt na zero
    std::vector<Edge*> parent(vertices); // poprzednicy

    cancelNegativeCycles();

    while (bellmanFord(source, sink, parent)) { // idziemy od ujścia do źródła / przechodzenie wstecz
        int pathFlow = std::numeric_limits<int>::max();

        

        for (int v = sink; v != source; v = parent[v]->from) {
            pathFlow = std::min(pathFlow, parent[v]->capacity - parent[v]->flow); // to samo co w maxFlow bierzemy najmniejszą wartość ze ścieżki
            //std::cout << "utknąłem" << std::endl;
        }

        for (int v = sink; v != source; v = parent[v]->from) { // aktualizacja przepływu wzdłuż ścieżki powiększającej, odejmowanie athFlow
            Edge* e = parent[v];
            e->flow += pathFlow;
            e->reverse->flow -= pathFlow;
            minrepairCost += e->repairCost; // dodanie kosztu naprawy drogi
        }

        maxFlow += pathFlow;
    }

    return {maxFlow, static_cast<int>(minrepairCost)}; // zwroć odpowiedzo
}

//do cwiartek

void ResidualNetwork::setCoordinates(int vertex, Point p) {
    coordinates[vertex] = {p.x, p.y};
} // ustawienie koordynatów punktu

//Wypisuje współrzędne wszystkich wierzchołków do stdout.
void ResidualNetwork::printCoordinates(){
    for (const auto& [id, coordin] : coordinates) {
        std::cout << "Wierzchołek " << id << ": (" << coordin.x << ", " << coordin.y << ")" << std::endl;
    }
}
