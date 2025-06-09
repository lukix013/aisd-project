#include "residual.h"



ResidualNetwork::ResidualNetwork(int vertices) : vertices(vertices) {
    this->mFlow = FLT_MAX;
    adjList.resize(vertices);
}
ResidualNetwork::ResidualNetwork(const ResidualNetwork& obj) {
    this->vertices = obj.vertices;
    this->mFlow = 0;
    this->adjList.resize(vertices);
    std::map<const Edge*, Edge*> OldNew;
    Edge* newEd;
    //poniewa¿ w adjList s¹ wskazniki do krawêdzi, trzeba siê troche nameczyc i zasadniczo zrobic 2 przejscia, zeby przypisac
    //reverse do istniejacych krawedzi
    for (int i = 0; i < this->vertices; i++) {
        for (Edge* oldEd : obj.adjList[i]) {
            newEd = new Edge(oldEd->from, oldEd->to, oldEd->capacity, oldEd->cost);
            newEd->flow = oldEd->flow;
            this->adjList[i].push_back(newEd);
            OldNew[oldEd] = newEd;
        }
    }
    //teraz trzeba zrobic odwrocone krawedzie
    for (int i = 0; i < this->vertices; i++) {
        for (Edge* oldEd : obj.adjList[i]) {
            newEd = OldNew.at(oldEd);
            auto it = OldNew.find(oldEd->reverse); //note: moze sie wywalic jesli nie ma reverse, ale bylby to blad w zalozonym grafie
            newEd->reverse = it->second;
        }
    }
}
ResidualNetwork::~ResidualNetwork() {
    for (int i = 0; i < this->vertices; i++) {
        for (Edge* kra : adjList[i]) {
            delete kra;
        }
    }
}
void ResidualNetwork::addEdges(std::string& filePath, int mode) {
    std::vector<float> Edges;
    zmodyfikujDane Stefan;
    Edges = Stefan.modDane(filePath, mode);
    //'debug'
    /*
std::cout << "Edges zawartosc:" << std::endl;
for (int i = 0; i < Edges.size(); ++i) {
    std::cout << Edges[i] << " ";
    if ((i + 1) % 3 == 0) {
        std::cout << std::endl;
    }
}
*/
    float d = 0;
    for (int i = 0; i < Edges.size(); i += 4) {
        //from, to, capacity, cost
        //std::cout << "Dodawanie krawedzi nr: " <<i<< std::endl; //tez do 'debugu'
        ResidualNetwork::addEdge((int)Edges[i], (int)Edges[i + 1], (Edges[i + 2]), (Edges[i + 3]));

    }
}
void ResidualNetwork::addEdge(int from, int to, float capacity, float cost) {
    Edge* forward = new Edge(from, to, capacity, cost);
    Edge* backward = new Edge(to, from, capacity, 0); // Trzeba jeszcze przemyslec czy cap idzie w druga strone, moze potencjalnie powodowac problemy w alg. ale chyba tego nie robi
    //tu ten brak kosztu w backward moze byc potem problemem, bo jesli o nim zapomnimy to bedziemy miec drogi 1-stronne
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
    this->resetFlow();
    float totalFlow = 0;
    std::vector<Edge*> parent(vertices);

    while (bfs(source, sink, parent)) {
        float pathFlow = std::numeric_limits<float>::max();

        // Znajdujemy najmniejsz¹ przepustowoœæ na œcie¿ce
        for (int v = sink; v != source; v = parent[v]->from) {
            pathFlow = std::min(pathFlow, parent[v]->capacity - parent[v]->flow);
        }

        // Aktualizujemy przep³ywy na œcie¿ce
        for (int v = sink; v != source; v = parent[v]->from) {
            Edge* edge = parent[v];
            edge->flow += pathFlow;
            edge->reverse->flow -= pathFlow;
        }

        totalFlow += pathFlow;
    }
    this->mFlow = totalFlow;
    return totalFlow;
}
void ResidualNetwork::resetFlow() {
    for (int i = 0; i < this->vertices; i++) {
        for (Edge* edge : adjList[i]) {
            edge->flow = 0;
        }
    }
    this->mFlow = 0;
}
bool ResidualNetwork::removeEdge(Edge* e1) {
    if (!e1 || !e1->reverse) return false;
    Edge* e2 = e1->reverse;
    auto it1 = std::find(adjList[e1->from].begin(), adjList[e1->from].end(), e1);
    if (it1 != adjList[e1->from].end()) {
        adjList[e1->from].erase(it1);
        delete e1;
    }
    else { return false; }

    auto it2 = std::find(adjList[e2->from].begin(), adjList[e2->from].end(), e2);
    if (it2 != adjList[e2->from].end()) {
        adjList[e2->from].erase(it2);
        delete e2;
    }
    else {
        return false;
    }
    return true;
}
ResidualNetwork ResidualNetwork::cheapest(int source, int sink) {
    //nie jest szczegolnie optymalne, i moze dzialac wolno dla duzych grafow
    this->maxFlow(source, sink);
    ResidualNetwork baseGraph = *this;
    float orgMaxFlow = this->mFlow;
    std::vector<Edge*> edgesWithCostInBase;
    for (int i = 0; i < baseGraph.vertices; i++) {
        for (Edge* edInBase : baseGraph.adjList[i]) {
            if (!edInBase) { std::cout<< "Error: Null pointer in baseGraph.adjList[" << i << "]." << std::endl; exit(1); }
            if (edInBase->cost > 0) {
                edgesWithCostInBase.push_back(edInBase);
            }

        }
    }
    if (edgesWithCostInBase.size() == 0) {
        return baseGraph;
    }

    float bestSavings = 0;
    ResidualNetwork bestVariant = *this;

    for (int i = 0; i < (1 << edgesWithCostInBase.size()); i++) {
        ResidualNetwork currVar = baseGraph;
        currVar.resetFlow();
        float currSavings = 0;

        std::vector<Edge*> edgesToRem;

        float eps = 1e-6; // z powodu ze uzywamy floatow, musimy sie spodziewac niedokladnosci, to jest nasza tolerancja
        for (int j = 0; j < edgesWithCostInBase.size(); j++) {
            if ((i >> j) & 1) { //uzywamy reprezentacji bitowej do generowania wariantow, to podejscie nie jest zbyt optymalne
                //poniewaz jesli np. jakas krawedz jest konieczna do osiagniecia maxFlow, to i tak bedziemy rozpatrzac przypadki bez niej
                Edge* edToRemInfo = edgesWithCostInBase[j];
                Edge* edToRem = nullptr;
                //poniewaz bedziemy usuwac krawedzie musimy szukac ich odpowiednikow
                //vector sie moze przesuwac, wiec musimy szukac, nie powinien nam 'uciec' ale lepiej tego nie zakladac
                for (Edge* edgeVariant : currVar.adjList[edToRemInfo->from]) {
                    if (edgeVariant->to == edToRemInfo->to &&
                        abs(edgeVariant->capacity - edToRemInfo->capacity) < eps &&
                        abs(edgeVariant->cost - edToRemInfo->cost) < eps) {
                        edToRem = edgeVariant;
                        break;
                    }
                }

                if (edToRem && edToRem->cost > 0) {
                    edgesToRem.push_back(edToRem);
                    currSavings += edToRem->cost;
                }
                else { //znalezlismy zla krawedz, blad
                    std::cerr << "Error: residual.cpp cheapest, could not val the edge" << std::endl;
                }
            }
        }
        bool removal_ok = true;
        for (Edge* edPtrVar : edgesToRem) {
            if (!currVar.removeEdge(edPtrVar)) {
                std::cerr << "Error in removing an edge" << std::endl;
                removal_ok = false;
                break;
            }
        }
        float variantMaxFlow = currVar.maxFlow(source, sink); //spr. czy wariant tez daje maxFlow


        if (std::abs(variantMaxFlow - orgMaxFlow) < eps) {
            
            if (currSavings > bestSavings) {
                bestSavings = currSavings;
                bestVariant = currVar;
            }
        }
    }
    
    return bestVariant;
}
ResidualNetwork& ResidualNetwork::operator=(const ResidualNetwork& other) {
    if (this == &other) return *this;

    for (int i = 0; i < this->vertices; i++) { 
        for (Edge* edPtr : this->adjList[i]) {
            delete edPtr; 
        }
        this->adjList[i].clear();
    }
    this->adjList.clear();

    this->vertices = other.vertices;
    this->mFlow = other.mFlow;
    this->adjList.resize(this->vertices);
	//podobnie jak w konstruktorze kopiujacym, trzeba zrobic 2 przejscia, zeby przypisac reverse do istniejacych krawedzi
    std::map<Edge*, Edge*> OldNew;
    for (int i = 0; i < this->vertices; i++) { 
        this->adjList[i].reserve(other.adjList[i].size());
        for (Edge* oldEd : other.adjList[i]) {
            if (!oldEd) continue;
            Edge* newEd = new Edge(oldEd->from, oldEd->to, oldEd->capacity, oldEd->cost);
            newEd->flow = oldEd->flow;
            this->adjList[i].push_back(newEd);
            OldNew[oldEd] = newEd;
        }
    }
    for (int i = 0; i < this->vertices; i++) {
        for (Edge* oldEd : other.adjList[i]) {
            if (!oldEd) continue;
            Edge* newerEd = OldNew.at(oldEd);
            if (oldEd->reverse) {
                auto it = OldNew.find(oldEd->reverse);
                if (it != OldNew.end()) {
                    newerEd->reverse = it->second;
                }
                else {
                    newerEd->reverse = nullptr;
                }
            }
            else {
                newerEd->reverse = nullptr;
            }
        }
    }
    return *this;
}
void ResidualNetwork::simplePrint() {
    for (int i = 0; i < this->vertices; i++) {
        for (ResidualNetwork::Edge* e : this->adjList[i]) {
            std::cout << e->from << " -> " << e->to << " : " << " cap: " << e->capacity << " flow : " << e->flow << " cost: " << e->cost << std::endl;
        }
        std::cout << std::endl;
    }
}

