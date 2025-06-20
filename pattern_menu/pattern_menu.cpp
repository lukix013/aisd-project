#include <iostream>
#include <locale>
#include <string>
#include <chrono>
#include "pattern_search.h"

int main(){
    std::setlocale(LC_ALL,".UTF8");

    std::string file;
    std::string pattern;

    std::cout << "Podaj nazwę pliku: ";
    std::cin >> file;
    //std::cout << std::endl;
    
    std::cout << "Podaj jakie słowo szukasz: ";
    std::cin >> pattern;
    //std::cout << std::endl;

    Pattern_search ps(file,pattern,true);

    std::cout << "Jaki algorytm chcesz użyć? Booyera Moore'a (BM)[0] czy Knutha Morrisa Pratta (KMP)[1]" << std::endl;
    bool ktory_alg;
    std::cin >> ktory_alg;
    auto start = std::chrono::high_resolution_clock::now();
    if(ktory_alg==0){
        std::cout << "Używam algorytmu Booyera Moore'a" << std::endl;
        //auto start = std::chrono::high_resolution_clock::now();
        ps.Boyer_Moor();
    }else if(ktory_alg==1){
        std::cout << "Używam algorytmu Knutha Morrisa Pratta" << std::endl;
        //auto start = std::chrono::high_resolution_clock::now();
        ps.KMP();
    }else{
        std::cout << "Nie podano właściwej wartości, używam Booyer'a Moore'a" << std::endl;
        //auto start = std::chrono::high_resolution_clock::now();
        ps.Boyer_Moor();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << std::endl;
    
    ps.print_highlighted();

    std::cout << "Czas wykonania: " << elapsed.count() << " ms" << std::endl;
    
    return 0;
}
