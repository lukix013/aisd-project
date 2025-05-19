#include <iostream>
#include "residual.cpp"
#include "zmodyfikujdane.cpp"


int main()
{
    int ile=6;
    std::string dane="dane.txt";
    ResidualNetwork Hubert(ile+2);
    std::cout << "ResudualNetwork creation ok" << std::endl;
    Hubert.addEdges(dane, 0);
    std::cout << "add Edges ok" << std::endl;
    std::cout<<Hubert.maxFlow(ile, ile+1)<<std::endl;;

	return 0;
}
