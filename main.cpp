#include <iostream>
#include "algortihms\data_generation.h"
#include "algortihms\residual.h"
#include "algortihms\pattern_search.h"
int main()
{
	//std::cout << "Hello" << std::endl;
	//Siec s("Sieci/siec.txt");
	//s.print_siec();
	//s.print_list();
	//ResidualNetwork rn(5);
	//DataGenerator dg;
	//dg.print_flow_network();
	//DataGenerator dg2(5);
	//dg2.print_flow_network();
	Pattern_search ps(std::string("testing_files\\wzorzec2.txt"),std::string("piwo"),true);
	//Pattern_search ps(std::string("piwo,jęczmień,browar"),std::string("piwo"));
	ps.Boyer_Moor();
	return 0;
}
