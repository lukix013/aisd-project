#include <iostream>
#include "Headers/siec.h"

int main()
{
	std::cout << "Hello" << std::endl;
	Siec s("Sieci/siec.txt");
	s.print_siec();
	s.print_list();
	return 0;
}
