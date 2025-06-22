#include "pattern_search.h"

//Konstruktor, w którym podajemy tekst(data) oraz wzorzec(pattern)
Pattern_search::Pattern_search(std::string data,std::string pattern){

    this->data = data;
    data_copy = this->data;
    std::transform(this->data.begin(),this->data.end(),this->data.begin(),::tolower);
    this->pattern = pattern;
    std::transform(this->pattern.begin(),this->pattern.end(),this->pattern.begin(),::tolower);

    //std::cout << data << std::endl;
}

//Konstruktor, w którym podajemy tekst,wzorzec oraz True lub False, gdy data to ścieżka do pliku
Pattern_search::Pattern_search(std::string data,std::string pattern,bool isFile){

    this->pattern = pattern;
    //Jeśli plik nie istnieje, kończy działanie aplikacji
    std::fstream file(data);
    if(!file.good()){
        std::cout << "Podany plik nie istnieje!" << std::endl;
        file.close();
        exit(0);
    }
    //Zapisuje tekst do zmiennej data
    if(isFile==true){
        std::fstream data_file(data);
        if(data_file.is_open()){
            std::string linia;
            /*while(data_file.good()){
                getline(data_file,this->data);
            }*/
           while(std::getline(data_file,linia)){
                this->data += linia + "\n";
           }
        }
        data_file.close();
    }
    //std::cout << this->data << std::endl;
    data_copy = this->data;
    std::transform(this->data.begin(),this->data.end(),this->data.begin(),::tolower);
    std::transform(this->pattern.begin(),this->pattern.end(),this->pattern.begin(),::tolower);
}

//Funkcja, która wykonuję algorytm Boyera Moore'a. Zwraca ona ile wzorców znaleźliśmy oraz dodaje do tablicy shift_locations przesunięcia wzorca
int Pattern_search::Boyer_Moor(){
    shift_locations.clear();
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

    std::map<char,int> LAST = Generate_LAST(AL,pattern); //Generuje tablicę LAST

    // for(auto k:LAST){
    //     std::cout << k.first << " " << k.second << " ";
    // }
    // std::cout << std::endl;

    std::vector<int> BMNext = Generate_BMNext(pattern); //Generuje tablicę BMNext

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
            //std::cout << "Wzorzec P wystepuje z przesunieciem " << i << std::endl;
            //Potencjalnie zapisac w tablicy gdzie te przesuniecia wystepuja?
            shift_locations.push_back(i);
            i = i + BMNext[0];
            wynik++;
        }
    }
    if(pp==0){
            //std::cout << "P Nie wystepuje w T" << std::endl;
        }

    return wynik;
}

//Funkcja ta generuję tablicę LAST do algorytmu Booyera-Moore'a
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

//Funkcja ta generuję tablicę BMNext do algorytmu Booyera-Moore'a
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

//Funkcja, która wykonuję algorytm KMP. Zwraca ona ile wzorców znaleźliśmy oraz dodaje do tablicy shift_locations przesunięcia wzorca
int Pattern_search::KMP(){
    shift_locations.clear();
    std::vector<int> pi = Generate_PI(pattern);
    int wynik=0;
    /*for(auto k:pi){
        std::cout << k << " ";
    }
    std::cout << std::endl;*/

    int q = 0;
    std::string& P = pattern;
    std::string& T = data;
    int n = T.size();
    int m = P.size();
    for(int i=0;i<n;i++){
        //std::cout << i << " " << q << std::endl;
        while(q>0 && P[q]!=T[i]){
            q=pi[q];
        }
        if(P[q]==T[i]){
            q=q+1;
            //std::cout << q << std::endl;
        }
        if(q==m){
            //std::cout << "Wzorzec wystepuje z przesunieciem " << i-m+1 << std::endl;
            shift_locations.push_back(i-m+1);
            q=pi[q-1];
            wynik++;
        }

    }

    return wynik;
}

//Funkcja ta generuję tablicę PI do algorytmu KMP.
std::vector<int> Pattern_search::Generate_PI(std::string P){
    int m = P.size();
    std::vector<int> pi(m,0);
    int k = 0;
    for(int q=1;q<m;q++){
        while(k>0 && P[k]!=P[q]){
            k=pi[k-1];
        }
        if(P[k]==P[q]){
            k=k+1;
        }
        pi[q]=k;
    }

    return pi;
}

//Funkcja ta zwraca przesunięcia wzorca w tekście
std::vector<int> Pattern_search::get_shift_locations(){
    return shift_locations;
}

//Funkcja ta drukuję w jednej lini przesunięcia wzroca w tekście.
void Pattern_search::print_shift_locations(){
    for(auto x: shift_locations){
        std::cout << x << " ";
    }
        std::cout << std::endl;
}

//Funkcja ta drukuję tekst, w którym szukamy wzorca
std::string Pattern_search::print_data(){
    std::cout << data << std::endl;

    return data;
}

//Funkcja ta drukuję tekst z wzorcami drukowanymi w kolorze czerwonym
void Pattern_search::print_highlighted(){
    const std::string RED   = "\033[31m"; //Kolor highlightu
    const std::string RESET = "\033[0m"; //Resetuj kolor

    int n = data_copy.size();
    std::vector<bool> color(n,false);

    // Zapisuje gdzie ma byc zaznaczony tekst
    for(auto k:shift_locations){
        for(int i=k; i<k+pattern.size() && i<n;++i){
            color[i]=true;
        }
    }

    //Drukuje znak po znaku
    //Gdy pojawi sie wzorzec, drukuje go w kolorze
    for(int i=0;i<n;++i){
        if(color[i] && (i==0 || !color[i-1])){
            std::cout << RED;
        }else if(!color[i] && i>0 && color[i-1]){
            std::cout << RESET;
        }
        std::cout << data_copy[i];
    }

    std::cout << RESET << std::endl;
}