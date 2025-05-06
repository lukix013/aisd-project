#include "residual.h"
#include <algorithm> // std::min

ResidualNetwork::ResidualNetwork(int vertices) : vertices(vertices) {
    adjList.resize(vertices);
}
void ResidualNetwork::addEdges(std::string& filePath, int mode){
    std::vector<int> Edges;
    zmodyfikujDane Stefan;
    Edges=Stefan.modDane(filePath, mode);
    for(int i=0;i<Edges.size();i+=3){
        ResidualNetwork::addEdge(Edges[i], Edges[i+1], float(Edges[i+2]));
    }
}
void ResidualNetwork::addEdge(int from, int to, float capacity) {
    Edge* forward = new Edge(from, to, capacity);
    Edge* backward = new Edge(to, from, 0); // Początkowo brak przepustowości w przeciwną stronę

    forward->reverse = backward;
    backward->reverse = forward;

    adjList[from].push_back(forward);
    adjList[to].push_back(backward);
}

bool ResidualNetwork::bfs(int source, int sink, std::vector<Edge*>& parent) {
    parent.assign(vertices, nullptr);
    std::vector<bool> visited(vertices, false);
    std::queue<int> q;

    q.push(source);
    visited[source] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (Edge* edge : adjList[current]) {
            if (!visited[edge->to] && edge->capacity > edge->flow) {
                parent[edge->to] = edge;
                visited[edge->to] = true;
                q.push(edge->to);

                if (edge->to == sink)
                    return true;
            }
        }
    }

    return false;
}

float ResidualNetwork::maxFlow(int source, int sink) {
    float totalFlow = 0;
    std::vector<Edge*> parent(vertices);

    while (bfs(source, sink, parent)) {
        float pathFlow = std::numeric_limits<float>::max();

        // Znajdujemy najmniejszą przepustowość na ścieżce
        for (int v = sink; v != source; v = parent[v]->from) {
            pathFlow = std::min(pathFlow, parent[v]->capacity - parent[v]->flow);
        }

        // Aktualizujemy przepływy na ścieżce
        for (int v = sink; v != source; v = parent[v]->from) {
            Edge* edge = parent[v];
            edge->flow += pathFlow;
            edge->reverse->flow -= pathFlow;
        }

        totalFlow += pathFlow;
    }

    return totalFlow;
}
