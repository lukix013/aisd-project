#include "data_generation.h"

DataGenerator::DataGenerator(){
    srand(time(0));
    int rand_number = rand() % 101;
    std::cout << rand_number << std::endl;
}

void DataGenerator::generate_flow_network(){
    
}