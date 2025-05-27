#ifndef PATTERN_SEARCH_H
#define PATTERN_SEARCH_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <set>

class Pattern_search{
    private:
        std::string pattern;
        std::string data;
        std::set<char> AL;
    public:
        Pattern_search(std::string data,std::string pattern);
        Pattern_search(std::string data,std::string pattern,bool isFile);
        std::vector<int> Generate_LAST(std::set<char> al);
        int Boyer_Moor(); //Zwracamy ile wzorców
};


#endif