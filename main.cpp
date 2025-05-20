#include <iostream>
#include "algortihms/data_generation.h"
#include "algortihms/residual.h"
int main()
{
	std::cout << "Hello" << std::endl;
	//Siec s("Sieci/siec.txt");
	//s.print_siec();
	//s.print_list();
	//ResidualNetwork rn(5);
	//DataGenerator dg;
	//dg.print_flow_network();
	DataGenerator dg2(5);
	dg2.print_flow_network();

	return 0;
}
