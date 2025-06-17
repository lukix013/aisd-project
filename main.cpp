#include <fstream>
#include <iostream>
#include <vector>
#include "residual.h"
#include "wyp.h"
#include "prz_wiel.h"

int main() {

    std::ifstream fin("input.txt");
    if(!fin){
        std::cout << "Błąd otwarcia pliku\n";
        return 1;
    }

    std::ofstream output("sprawozdanie.txt");

    int quartersCount;
    fin >> quartersCount;

    std::vector<int> quartersValues(quartersCount);

    for (int i = 0; i < quartersCount; i++) {
        fin >> quartersValues[i];
    }

    std::vector<std::vector<Point>> quarters(quartersCount);

    for (int i = 0; i < quartersCount; i++) {
        int pointsCount;

        fin >> pointsCount;
        quarters[i].resize(pointsCount);

        for (int j = 0; j < pointsCount; j++) {
            fin >> quarters[i][j].x >> quarters[i][j].y;
        }
    }

    for (auto &quarter : quarters) {
        convexHull(quarter);
    }

    int fieldCount, browCount;
    fin >> fieldCount;
    fin >> browCount;


    int V1;
    fin >> V1;

    ResidualNetwork net1(V1);

    for (int i = 0; i < fieldCount; i++) {
        Point p;
        fin >> p.x >> p.y;
        net1.setCoordinates(i, p);
    }

    int source1, sink1;
    fin >> source1 >> sink1;

    std::vector<int> poleProd(fieldCount, 0);
    for (int i = 0; i < fieldCount; i++) {
        for (int j = 0; j < quartersCount; j++) {
            if (punkt_w_wielokacie(net1.coordinates[i], quarters[j])) {
                poleProd[i] += quartersValues[j];
            }
        }
    }

    for (int i = 0; i < fieldCount; i++) {
        net1.addEdge(source1, i, poleProd[i], 0);
    }

    int edgesCount1;
    fin >> edgesCount1;

    for (int i = 0; i < edgesCount1; i++) {
        int u, v;
        int capacity;
        int cost;
        fin >> u >> v >> capacity >> cost;
        net1.addEdge(u, v, capacity, cost);
    }

    auto [flow1, cost1] = net1.minCostMaxFlow(source1, sink1);
    output << "Wasza Dostojność Samwise, \nPo długim i skrupulatnym badaniu pól, browaróœ, karczm oraz dróg Shire, zespół techniczny wspomagany komputerem przygotował plan zapewnienia dostatku piwa w każdej karczmie. \nEtap pierwszy\nPola iprawne zostały zbadane przez kartografóœ i botaników. Okazało się, że w zależności od ćwiartki kraju, w której leży dane pole, plony różnią się znacząco.\nNastępnie uruchomiliśmy transporty jęczmienia do browaróœ. Drogi były trudne, część z nich wymagała kosztownych napraw, ale nasze wozy zdołały dotrzeć do celu.\nUdało się przetransportować: " << flow1 << ", łączny koszt naprawy dróg wyniósł:" << cost1 << "\n";

    output << "W poszczególnych browarach znalazło się: \n";
    std::vector<int> jeczmienBrowary(4, 0);
    for (int i = V1 - browCount - 3; i < V1 - 3; i++) {
        for (Edge* e : net1.adjList[i]) {
            if(e->to == sink1){
                jeczmienBrowary[i - 8] = e->flow;
                output << "Jeczmien w browarze " << (i - 7) << ": " << e->flow << "\n";
            }
        
        }
    }

    output << "Etap drugi\nGdy tylko browary rozpoczęły warzenie, zapach świeżego piwa zaczął unosić się w powietrzu. Karczmarze z niecierpliwością wyglądali dostaw. Z browarów piwo ruszyło w emocjonującą podróż do naszych wspaniałych karczm.\nTransport piwa poszedł sprawnie, udało się dostarczyć: ";

    int V2;
    fin >> V2;
    ResidualNetwork net2(V2);

    int source2, sink2;
    fin >> source2 >> sink2;

    for (int i = 0; i < 4; i++) {
        net2.addEdge(source2, 8 + i, jeczmienBrowary[i], 0);
    }

    int edgesCount2;
    fin >> edgesCount2;

    for (int i = 0; i < edgesCount2; i++) {
        int u, v;
        int capacity;
        int cost;

        fin >> u >> v >> capacity >> cost;
        net2.addEdge(u, v, capacity, cost);

    }

    auto [flow2, cost2] = net2.minCostMaxFlow(source2, sink2);
    output << flow2 << ", koszt naprawy dróg wyniósł: " << cost2 << "\n";

    output << "Burmistrzu Samwise, dzięki Waszej mądrości i gościnności każdy hobbit w Shire będzie mógł wznieść świeżego piwa.\nZ szacunkiem i kuflem zminego piwa,\nTechniczny Zespół Hobbitonu.";

    fin.close();
    output.close();
   
    return 0;
}
