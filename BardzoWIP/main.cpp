#include <iostream>
#include "residual.h"


int main()
{
    int ile=8;
    std::string dane="dane.txt";
    ResidualNetwork Hubert(ile);
    Hubert.addEdges(dane, 0);
    Hubert.maxFlow(ile, ile+1);

	return 0;
}
