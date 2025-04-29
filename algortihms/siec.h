#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

struct Data{
int max_flow=0;
int current=0;
int feedback=0; //zwrotny
bool visited=false;
};

class Siec{
private:
    vector<vector<Data>> macierz;
    vector<vector<int>> lista_s; //lista sasiadow
    int N;
public:
    Siec(string plik_nazwa){
        int i=0;
        int n=0;
        ifstream plik(plik_nazwa);
        while(true){
            if(i==0){
                plik>>n;
                cout << n << endl;
                N=n;
                i++;
                for(int j=0;j<n;j++){
                    vector<Data> temp;
                    for(int k=0;k<n;k++){
                        Data d;
                        temp.push_back(d);
                        vector<int> temp;
                        lista_s.push_back(temp);
                    }
                    macierz.push_back(temp);
                }
            }else{
                int w,s,waga;
                plik>>w; //wierzcholek
                plik>>s; //sasiad
                plik>>waga; //waga

                Data plik_d;
                plik_d.max_flow=waga;
                plik_d.current=waga;
                plik_d.feedback=0;
                macierz[w][s]=plik_d;

                lista_s[w].push_back(s);

            }
            if(plik.eof()){
                break;
            }
        }
        plik.close();
    }

    void print_siec(){
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                cout << macierz[i][j].max_flow << " ";
            }
            cout << endl;
        }
    }

    void print_list(){
        for(int i=0;i<lista_s.size();i++){
            for(int j=0;j<lista_s[i].size();j++){
                cout << lista_s[i][j] << " ";
            }
            cout << endl;
        }
    }

    void BFS(){
        queue<int> q;
        q.push(0);
        while(!q.empty()){

        }
    }

    ~Siec(){
        //WYCZYSZCZENIE WEKTORA macierz
    }
};
