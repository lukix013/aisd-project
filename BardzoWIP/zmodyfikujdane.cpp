#include "zmodyfikujdane.h"
/*
wejscie do funkcji: txt z danymi, tryb (0-z pol do browarow, 1-z browarow do tawern)
Dane wejsciowe txt:
l. orginalnych wektorow
ile pol, (nr. pola, prod)...
ile browarow, (nr, poj/prod)...
ile tawern (nr, poj)...
ile zwyklych skrzyzowan
(skad, dokad, poj, koszt
*/
std::vector<float> zmodyfikujDane::modDane(std::string& filePath, int mode) {

	//warto zwrocic uwage, ze f. tworzy sztuczne krawedzie z zrodel i miejsc docelowych i one kosztu nie maja

    if (mode != 0 && mode != 1) {
        throw std::invalid_argument("Tryb musi byc 1, albo 0");
    }

    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Nie udalo sie otworzyc pliku " + filePath);
    }

    std::vector<float> edges;
    std::string line;
    float N; // l. oryginalnych wierzcholkow

    if (!std::getline(inputFile, line)) {
        throw std::runtime_error("Problem w czytaniu ilosci wierzcholkow");
    }
    std::stringstream ssN(line);
    if (!(ssN >> N) || N < 0) {
        throw std::runtime_error("Niewlasciwa ilosc wierzcholkow");
    }
    float source = N;
    float sink = N + 1;

    try {
        if (!std::getline(inputFile, line)) throw std::runtime_error("Problem w czytaniu pola");
        std::stringstream ss1(line);
        float n1; //numer pola
        if (!(ss1 >> n1) || n1 < 0) throw std::runtime_error("Problem w czytaniu pola " + std::to_string(n1));
        for (int i = 0; i < (int)n1; i++) {
            float v, cap;
            if (!(ss1 >> v >> cap)) throw std::runtime_error("Problem w czytaniu pola" + std::to_string(n1));
            if (v < 0 || v >= N) throw std::out_of_range("Pole poza zakresem " + std::to_string(v));

            if (mode == 0 && cap > 0) {
                edges.push_back(source);
                edges.push_back(v);
                edges.push_back(cap); //zakladam ze drogi maja przepustowosc calkowita
                edges.push_back(0); //to sa "sztuczne" drogi wiec nie musimy ich naprawiac
            }
        }

        if (!std::getline(inputFile, line)) throw std::runtime_error("Problem w czytaniu browaru");
        std::stringstream ss2(line);
        float n2;
        if (!(ss2 >> n2) || n2 < 0) throw std::runtime_error("Problem w czytaniu browaru");
        for (int i = 0; i < n2; ++i) {
            float v, cap;
            if (!(ss2 >> v >> cap)) throw std::runtime_error("Problem w czytaniu browaru");
            if (v < 0 || v >= N) throw std::out_of_range("Browar poza zakresem " + std::to_string(v));

            if (cap > 0) {
                if (mode == 0) { //Browary sa miejscami docelowymi
                    edges.push_back(v);
                    edges.push_back(sink);
                    edges.push_back(cap);
                    edges.push_back(0); //tez "sztuczne"
                }
                else { //Browary sa zrodlami
                    edges.push_back(source);
                    edges.push_back(v);
                    edges.push_back(cap);
                    edges.push_back(0); //tez "sztuczne"
                }
            }
        }

        if (!std::getline(inputFile, line)) throw std::runtime_error("Problem w czytaniu tawerny");
        std::stringstream ss3(line);
        float n3;
        if (!(ss3 >> n3) || n3 < 0) throw std::runtime_error("Problem w czytaniu tawerny");
        for (int i = 0; i < n3; i++) {
            float v, cap;
            if (!(ss3 >> v >> cap)) throw std::runtime_error("Problem w czytaniu tawerny");
            if (v < 0 || v >= N) throw std::out_of_range("Tawerna poza zakresem:  " + std::to_string(v));

            if (mode == 1 && cap > 0) {
                edges.push_back(v);
                edges.push_back(sink);
                edges.push_back(cap);
                edges.push_back(0); //tez "sztuczne"
            }
        }
    }
    catch (const std::exception& e) {
        inputFile.close(); //trzeba zawsze pamietac o zamknieciu pliku
        throw; //oddaje blad spowrotem
    }

    float n4; //ile skrzyzowan
    if (std::getline(inputFile, line)) {
        std::stringstream ss4(line);
        if (!(ss4 >> n4) || n4 < 0) throw std::runtime_error("niewlasciwy format");
    }
    try {
        for (int i = 0; i < n4; i++) {
            if (!std::getline(inputFile, line)) {
                throw std::runtime_error("Problem w czytaniu linii " + std::to_string(i + 1));
            }
            std::stringstream ssEdge(line);
            float u, v, capacity, cost;
            if (!(ssEdge >> u >> v >> capacity >> cost)) {
                throw std::runtime_error("Niewlasciwy format " + std::to_string(i + 1));
            }

            if (u < 0 || u >= N || v < 0 || v >= N) {
                throw std::out_of_range("Poza zakresem: u=" + std::to_string(u) + " v=" + std::to_string(v));
            }

            // Dodaje krawedz jesli ma pojemnosc, ujemne krawedzie ma sens robic osobno
            if (capacity > 0) {
                edges.push_back(u);
                edges.push_back(v);
                edges.push_back(capacity);
                edges.push_back(cost);
            }
        }
    }
    catch (const std::exception& e) {
        inputFile.close(); //trzeba zawsze pamietac o zamknieciu pliku
        throw; //oddaje blad spowrotem
    }
    inputFile.close();
    return edges;
}

