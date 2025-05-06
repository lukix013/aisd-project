#include "zmodyfikujdane.h"
/*
wejscie do funkcji: txt z danymi, tryb (0-z pol do browarow, 1-z browarow do tawern)
Dane wejsciowe txt:
l. orginalnych wektorow
ile pol, (nr. pola, prod)
ile browarow, (nr, poj/prod)
ile tawern (nr, poj)
ile zwyklych skrzyzowan*/
std::vector<int> zmodyfikujDane::modDane(std::string& filePath, int mode) {

    if (mode != 0 && mode != 1) {
        throw std::invalid_argument("Musi byc 1, albo 0");
    }

    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Nie udalo sie otworzyc pliku " + filePath);
    }

    std::vector<int> edges;
    std::string line;
    int N;

    if (!std::getline(inputFile, line)) {
        throw std::runtime_error("Problem w czytaniu ilosci wektorow");
    }
    std::stringstream ssN(line);
    if (!(ssN >> N) || N < 0) {
        throw std::runtime_error("Niewlasciwa ilosc verteksow");
    }
    int source = N;
    int sink = N + 1;

    try {
        if (!std::getline(inputFile, line)) throw std::runtime_error("Problem w czytaniu pola");
        std::stringstream ss1(line);
        int n1;
        if (!(ss1 >> n1) || n1 < 0) throw std::runtime_error("Problem w czytaniu pola");
        for (int i = 0; i < n1; ++i) {
            int v, cap;
            if (!(ss1 >> v >> cap)) throw std::runtime_error("Problem w czytaniu pola");
            if (v < 0 || v >= N) throw std::out_of_range("Pole poza zakresem " + std::to_string(v));

            if (mode == 0 && cap > 0) {
                edges.push_back(source);
                edges.push_back(v);
                edges.push_back(cap); //zakladam ze drogi maja przepustowosc calkowita
            }
        }

        if (!std::getline(inputFile, line)) throw std::runtime_error("Problem w czytaniu browaru");
        std::stringstream ss2(line);
        int n2;
        if (!(ss2 >> n2) || n2 < 0) throw std::runtime_error("Problem w czytaniu browaru");
        for (int i = 0; i < n2; ++i) {
            int v, cap;
            if (!(ss2 >> v >> cap)) throw std::runtime_error("Problem w czytaniu browaru");
            if (v < 0 || v >= N) throw std::out_of_range("Browar poza zakresem " + std::to_string(v));

            if (cap > 0) {
                if (mode == 0) {
                    edges.push_back(v);
                    edges.push_back(sink);
                    edges.push_back(cap);
                } else {
                    edges.push_back(source);
                    edges.push_back(v);
                    edges.push_back(cap);
                }
            }
        }

        if (!std::getline(inputFile, line)) throw std::runtime_error("Problem w czytaniu tawerny");
        std::stringstream ss3(line);
        int n3;
        if (!(ss3 >> n3) || n3 < 0) throw std::runtime_error("Problem w czytaniu tawerny");
        for (int i = 0; i < n3; ++i) {
            int v, cap;
            if (!(ss3 >> v >> cap)) throw std::runtime_error("Problem w czytaniu tawerny");
            if (v < 0 || v >= N) throw std::out_of_range("Tawerna poza zakresem:  " + std::to_string(v));

            if (mode == 1 && cap > 0) {
                edges.push_back(v);
                edges.push_back(sink);
                edges.push_back(cap);
            }
            }
        }
        catch (const std::exception& e) {
        inputFile.close();
        throw;
    }
        if (std::getline(inputFile, line)){
        std::stringstream ss4(line);
        int n4;
        if (!(ss4 >> n4) || n4 < 0) throw std::runtime_error("niewlasciwy format");
        for (int i = 0; i < n4; ++i) {
            int v;
            if (!(ss4 >> v)) throw std::runtime_error("Skrzyzowanie problem w czytaniu");
            if (v < 0 || v >= N) throw std::out_of_range("Skrzyzowanie poza zakresem " + std::to_string(v));
        }
        }

        int M; // ile lini
        if (!std::getline(inputFile, line)) {
            throw std::runtime_error("Error reading number of edges (M) from file.");
        }
        std::stringstream ssM(line);
        if (!(ssM >> M) || M < 0) {
            throw std::runtime_error("Invalid format for number of edges (M).");
        }

        for (int i = 0; i < M; ++i) {
            if (!std::getline(inputFile, line)) {
                throw std::runtime_error("Problem w czytaniu linii " + std::to_string(i + 1) + ".");
            }
            std::stringstream ssEdge(line);
            int u, v, capacity;
            if (!(ssEdge >> u >> v >> capacity)) {
                throw std::runtime_error("Niewlasciwy format " + std::to_string(i + 1) + ".");
            }

            if (u < 0 || u >= N || v < 0 || v >= N) {
                 throw std::out_of_range("Poza zakresem: u=" + std::to_string(u) + ", v=" + std::to_string(v));
            }

            // Dodaje krawedz jesli ma pojemnosc, ujemne krawedzie ma sens robic osobno
            if (capacity > 0) {
                edges.push_back(u);
                edges.push_back(v);
                edges.push_back(capacity);
            }
        }

    inputFile.close();
    return edges;
}

