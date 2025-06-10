#include "pattern_search.h"

Pattern_search::Pattern_search(std::string data,std::string pattern){
    //setlocale(LC_ALL, "pl_PL");

    this->data = data;
    std::transform(this->data.begin(),this->data.end(),this->data.begin(),::toupper);
    this->pattern = pattern;
    std::transform(this->pattern.begin(),this->pattern.end(),this->pattern.begin(),::toupper);

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
    //std::cout << this->data << std::endl;
    std::transform(this->data.begin(),this->data.end(),this->data.begin(),::toupper);
    std::transform(this->pattern.begin(),this->pattern.end(),this->pattern.begin(),::toupper);
}

int Pattern_search::Boyer_Moor(){
    int wynik=0;
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

    std::map<char,int> LAST = Generate_LAST(AL,pattern); 

    // for(auto k:LAST){
    //     std::cout << k.first << " " << k.second << " ";
    // }
    // std::cout << std::endl;

    std::vector<int> BMNext = Generate_BMNext(pattern);

    // for(auto k:BMNext){
    //     std::cout << k << " ";
    // }
    // std::cout << std::endl;

    std::string& P = pattern;
    std::string& T = data;
    int m = P.size();
    int n = T.size();
    //std::cout << P << " " << T << std::endl;
    int pp = 0;
    int i = 0;
    while(i<=n-m){
        int j = m-1;
        while(j>=0 && (P[j]==T[i+j])){
            j=j-1;
        }
        if(j>=0){
            i=i+std::max(BMNext[j+1],j-LAST[T[i+j]]);
        }else{
            pp=i+1;
            std::cout << "Wzorzec P wystepuje z przesunieciem " << i << std::endl;
            //Potencjalnie zapisac w tablicy gdzie te przesuniecia wystepuja?
            i = i + BMNext[0];
        }
    }
    if(pp==0){
            std::cout << "P Nie wystepuje w T" << std::endl;
        }

    return wynik;
}

std::map<char,int> Pattern_search::Generate_LAST(std::set<char> al,std::string P){
    std::map<char,int> LAST;

    for(auto x:al){
        LAST[x]=0;
    }

    for(int i=0;i<P.size();i++){
        LAST[P[i]]=i;
    }

    return LAST;
}

std::vector<int> Pattern_search::Generate_BMNext(std::string P){
    int m = P.size();
    std::vector<int> BMNext(m+1,0);
    std::vector<int> PI(m+1,0);
    int i = m;
    int b = m+1;
    PI[i]=b;
    while(i>=1){
        while(b<=m && (P[i-1]!=P[b-1])){
            if(BMNext[b]==0){
                BMNext[b]=b-i;
            }
            b=PI[b];
        }
        b=b-1;
        i=i-1;
        PI[i]=b;
    }

    // for(auto k:PI){
    //     std::cout << k << " ";
    // }
    // std::cout << std::endl;

    b = PI[0];
    for(int i=0;i<m+1;i++){
        if(BMNext[i]==0){
            BMNext[i]=b-1;
        }
        if(i==b){
            b=PI[b];
        }
    }

    //BMNext[0]=1;

    return BMNext;
}