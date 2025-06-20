#ifndef PATTERN_SEARCH_H
#define PATTERN_SEARCH_H
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include <chrono>

class Pattern_search{
    private:
        std::string pattern;
        std::string data;
        std::string data_copy;
        std::set<char> AL;
        //std::vector<int> BMNext;
        std::vector<int> shift_locations;
    public:
        Pattern_search(std::string data,std::string pattern);
        Pattern_search(std::string data,std::string pattern,bool isFile);
        std::map<char,int> Generate_LAST(std::set<char> al,std::string P);
        int Boyer_Moor(); //Zwracamy ile wzorców
        std::vector<int> Generate_BMNext(std::string P);
        int KMP();
        std::vector<int> Generate_PI(std::string P);
        std::vector<int> get_shift_locations();
        void print_shift_locations();
        std::string print_data();
        void print_highlighted();
};


#endif