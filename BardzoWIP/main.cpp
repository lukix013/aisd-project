
#include <iostream>
#include "residual.h"


int main()
{
    //testowanie, przykladowe uzycie
    int ile = 6;
    int source = 6;
	int sink = 7;
    std::string dane = "dane.txt";
    ResidualNetwork Hubert(ile + 2);
    std::cout << "ResudualNetwork creation ok" << std::endl;
    Hubert.addEdges(dane, 0);
    std::cout << "add Edges ok, Max Flow bazowo:" << std::endl;
    std::cout << Hubert.maxFlow(source, sink) << std::endl;
    Hubert.simplePrint();
    /*
    * to jest teraz w simplePrint
    std::cout << "num. of vertices:" << Hubert.vertices << std::endl;
    for (int i = 0; i < ile + 2; i++) {
        for (ResidualNetwork::Edge* e : Hubert.adjList[i]) {
            std::cout << e->from << " -> " << e->to << " : " << " cap: " << e->capacity << " flow : " << e->flow << " cost: " <<e->cost << std::endl;
        }
        std::cout << std::endl;
    }
    */
	std::cout << "Najtanszy:" << std::endl;
	Hubert = Hubert.cheapest(source, sink);
	std::cout << "Cheapest ok" << std::endl;
    //std::cout << "num. of vertices:" << Hubert.vertices << std::endl;
    std::cout << "Max flow w najtanszym:" << std::endl;
    std::cout << Hubert.maxFlow(source, sink) << std::endl;
    Hubert.simplePrint();
    
    return 0;
}
