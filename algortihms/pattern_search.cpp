#include "pattern_search.h"

Pattern_search::Pattern_search(std::string data,std::string pattern){
    //setlocale(LC_ALL, "pl_PL");

    this->data = data;
    this->pattern = pattern;

    std::cout << data << std::endl;
}

// Konstruktor z zapytaniem, czy data to nazwa pliku
Pattern_search::Pattern_search(std::string data,std::string pattern,bool isFile){
    //std::locale::global(std::locale("pl_PL.UTF-8")); 
    this->pattern = pattern;

    if(isFile==true){
        std::fstream data_file(data);
        if(data_file.is_open()){
            while(data_file.good()){
                getline(data_file,this->data);
            }
        }
        data_file.close();
    }
    std::cout << this->data << std::endl;
}

int Pattern_search::Boyer_Moor(){
    int n=0;
    //Czyta alfabet tekstu
    for(char c: data){
        if(isalpha(c)){
            AL.insert(c);
        }
    }

    /*for(char c: AL){
        std::cout << c << " ";
    }
    std::cout << std::endl;*/


    return n;
}

std::vector<int> Pattern_search::Generate_LAST(std::set<char> al){
    
}