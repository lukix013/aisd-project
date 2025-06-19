#include <iostream>
#include <locale>
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
	std::setlocale(LC_ALL,".UTF8");
	//Pattern_search ps(std::string("testing_files\\wzorzec2.txt"),std::string("piwo"),true);
	Pattern_search ps(std::string("testing_files\\raport.txt"),std::string("browar"),true);
	//Pattern_search ps(std::string("AAACABABACABBB"),std::string("ABABACA"));
	ps.Boyer_Moor();
	//ps.print_shift_locations();
	ps.KMP();
	//ps.print_shift_locations();
	//ps.print_data();
	ps.print_highlighted();
	return 0;
}
