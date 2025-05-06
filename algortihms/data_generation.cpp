#include "data_generation.h"

DataGenerator::DataGenerator(){
    srand(time(0));
    int rand_number = rand() % 11;
    points = rand_number;
    std::cout << "Will generate: " << points << std::endl;
    generate_flow_network();
}

DataGenerator::DataGenerator(int points){
    this->points = points;
    std::cout << points << std::endl;
    generate_flow_network();
}

void DataGenerator::generate_flow_network(){
    for(int i=0;i<points;i++){
        int rand_capacity = rand() % 21;
        int rand_flow = rand() % rand_capacity;
        std::vector<int> temp;
        temp.push_back(rand_capacity);
        temp.push_back(rand_flow);
        AdjList.push_back(temp);
    }
}

void DataGenerator::print_flow_network(){
    for(int i=0;i<AdjList.size();i++){
        for(int j=0;j<AdjList[i].size();j++){
            std::cout << AdjList[i][j] << " ";
        }
        std::cout << std::endl;
    }
}