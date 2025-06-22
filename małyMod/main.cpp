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

    std::vector<int> quartersValues(quartersCount); //Wartości plonów w poszczególnych ćwiartkach.

    for (int i = 0; i < quartersCount; i++) {
        fin >> quartersValues[i];
    }

    std::vector<std::vector<Point>> quarters(quartersCount); // Współrzędne granic ćwiartek.

    for (int i = 0; i < quartersCount; i++) {
        int pointsCount;
        fin >> pointsCount;
        quarters[i].resize(pointsCount);

        for (int j = 0; j < pointsCount; j++) {
            fin >> quarters[i][j].x >> quarters[i][j].y;
        }
    }

    // Obliczanie otoczki wypukłej dla każdej ćwiartki
    for (auto &quarter : quarters) {
        convexHull(quarter);
    }

    int fieldCount, browCount;
    fin >> fieldCount;
    fin >> browCount;

    int V1; // Liczba wierzchołków w pierwszej sieci
    fin >> V1;

    ResidualNetwork net1(V1); //siec 1 dla pierwszej fazy

    for (int i = 0; i < fieldCount; i++) {
        Point p;
        fin >> p.x >> p.y;
        net1.setCoordinates(i, p);
    }

    int source1, sink1;
    fin >> source1 >> sink1;

    std::vector<int> poleProd(fieldCount, 0); //Produkcja jęczmienia na poszczególnych polach.

    // Wyznaczanie produkcji jęczmienia na podstawie lokalizacji pól względem ćwiartek
    for (int i = 0; i < fieldCount; i++) {
        for (int j = 0; j < quartersCount; j++) {
            if (punkt_w_wielokacie(net1.coordinates[i], quarters[j])) {
                poleProd[i] += quartersValues[j];
                std::cout << quartersValues[j] << std::endl;
            }
        }
    }

    // Dodanie krawędzi z pól do źródła w grafie
    for (int i = 0; i < fieldCount; i++) {
        net1.addEdge(source1, i, poleProd[i], 0);
    }

    int edgesCount1;
    fin >> edgesCount1;

    // Dodanie krawędzi między polami a browarami
    for (int i = 0; i < edgesCount1; i++) {
        int u, v;
        int capacity;
        int cost;
        fin >> u >> v >> capacity >> cost;
        net1.addEdge(u, v, capacity, cost);
    }

    // Obliczenie maksymalnego przepływu o minimalnym koszcie
    auto [flow1, cost1] = net1.minCostMaxFlow(source1, sink1);

    output << "Wasza Dostojność Samwise, \nPo długim i skrupulatnym badaniu pól, browaróœ, karczm oraz dróg Shire, zespół techniczny wspomagany komputerem przygotował plan zapewnienia dostatku piwa w każdej karczmie. \nEtap pierwszy\nPola iprawne zostały zbadane przez kartografóœ i botaników. Okazało się, że w zależności od ćwiartki kraju, w której leży dane pole, plony różnią się znacząco.\nNastępnie uruchomiliśmy transporty jęczmienia do browaróœ. Drogi były trudne, część z nich wymagała kosztownych napraw, ale nasze wozy zdołały dotrzeć do celu.\nUdało się przetransportować: " << flow1 << ", łączny koszt naprawy dróg wyniósł:" << cost1 << "\n";

    output << "W poszczególnych browarach znalazło się: \n";

    std::vector<int> jeczmienBrowary(browCount, 0); //Ilość jęczmienia w poszczególnych browarach.


    std::cout << browCount << std::endl;

    int browStartIndex = V1 - browCount - 3;

    // Odczyt przepływu jęczmienia do browarów
    for (int i = browStartIndex; i < browStartIndex + browCount + 1; i++) {

        for (Edge* e : net1.adjList[i]) {
            if(e->to == sink1){
                jeczmienBrowary[i - browStartIndex] = e->flow;
                output << "Jeczmien w browarze " << (i - browStartIndex) << ": " << e->flow << "\n";
            }
        }
    }

    output << "Etap drugi\nGdy tylko browary rozpoczęły warzenie, zapach świeżego piwa zaczął unosić się w powietrzu. Karczmarze z niecierpliwością wyglądali dostaw. Z browarów piwo ruszyło w emocjonującą podróż do naszych wspaniałych karczm.\nTransport piwa poszedł sprawnie, udało się dostarczyć: ";

    int V2;
    fin >> V2;
    ResidualNetwork net2(V2); //Sieć 2 dla fzy 2

    int source2, sink2;
    fin >> source2 >> sink2;

    // Dodanie krawędzi z browarów do źródła w drugiej sieci
    for (int i = 0; i < browCount; i++) {
        net2.addEdge(source2, 8 + i, jeczmienBrowary[i], 0);
    }

    int edgesCount2;
    fin >> edgesCount2;

    // Dodanie krawędzi między browarami a karczmami
    for (int i = 0; i < edgesCount2; i++) {
        int u, v;
        int capacity;
        int cost;

        fin >> u >> v >> capacity >> cost;
        net2.addEdge(u, v, capacity, cost);
    }

    // Obliczenie przepływu z browarów do karczm
    auto [flow2, cost2] = net2.minCostMaxFlow(source2, sink2);

    // Zapis wyników drugiego etapu do pliku
    output << flow2 << ", koszt naprawy dróg wyniósł: " << cost2 << "\n";
    output << "Burmistrzu Samwise, dzięki Waszej mądrości i gościnności każdy hobbit w Shire będzie mógł wznieść świeżego piwa.\nZ szacunkiem i kuflem zminego piwa,\nTechniczny Zespół Hobbitonu.";

    fin.close();
    output.close();
   
    return 0;
}
