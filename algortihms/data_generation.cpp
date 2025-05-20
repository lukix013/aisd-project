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
    std::cout << "Will generate: " << this->points << std::endl;
    generate_flow_network();
}

void DataGenerator::generate_flow_network(){
    for(int i=0;i<points;i++){
        std::vector<int> temp;
        AdjList.push_back(temp);
    }

    std::set<std::pair<int,int>> uzyte;
    srand(time(0));
    int k=0;
    while(k<points){
        int rand_capacity = rand() % 20 + 1;
        int rand_flow = rand_capacity;
        std::vector<int> temp;
        int u = rand() % points;
        int v = rand() % points;
        if(u==v || uzyte.count({u,v}) || uzyte.count({v,u}) || v==0 || u==points-1){
            continue;
        }
        temp.push_back(u);
        temp.push_back(v);
        //temp.push_back(rand_capacity);
        //temp.push_back(rand_flow);
        AdjList.push_back(temp);
        uzyte.insert({u,v});
        k++;
    }
}

//Uzycie BFS do sprawdzenia czy istnieje droga z zrodla do celu
bool DataGenerator::road_exists(int s,int t){
    std::vector<bool> visited(points,false);
    std::queue<int> q;
    q.push(s);
    visited[s]=true;

    while(!q.empty()){
        int u = q.front();q.pop();
        if(u == t){
            return true;
        }
        
    }

    return false;
}

void DataGenerator::print_flow_network(){
    for(int i=0;i<AdjList.size();i++){
        for(int j=0;j<AdjList[i].size();j++){
            std::cout << AdjList[i][j] << " ";
        }
        std::cout << std::endl;
    }
}