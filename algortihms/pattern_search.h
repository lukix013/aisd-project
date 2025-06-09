#ifndef PATTERN_SEARCH_H
#define PATTERN_SEARCH_H
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <map>

class Pattern_search{
    private:
        std::string pattern;
        std::string data;
        std::set<char> AL;
        //std::vector<int> BMNext;
    public:
        Pattern_search(std::string data,std::string pattern);
        Pattern_search(std::string data,std::string pattern,bool isFile);
        std::map<char,int> Generate_LAST(std::set<char> al,std::string P);
        int Boyer_Moor(); //Zwracamy ile wzorców
        std::vector<int> Generate_BMNext(std::string P);
};


#endif