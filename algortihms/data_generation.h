#ifndef DATA_GENERATION_H
#define DATA_GENERATION_H

#include <iostream>
#include "residual.h"
#include <cstdlib>
#include <ctime>
#include <vector>

class DataGenerator{
public:
    DataGenerator();
    DataGenerator(int points);
    void generate_flow_network();
    void print_flow_network();
private:
    int points; //Ile punktow(s i t wlacznie?)
    std::vector<std::vector<int>> AdjList;
};

#endif