#ifndef ZMODYFIKUJDANE_H
#define ZMODYFIKUJDANE_H
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
/*
wejscie do funkcji: txt z danymi, tryb (0-z pol do browarow, 1-z browarow do tawern)
Dane wejsciowe txt:
l. orginalnych wektorow
ile pol, (nr. pola, prod)
ile browarow, (nr, poj/prod)
ile tawern (nr, poj)
ile zwyklych skrzyzowan*/
class zmodyfikujDane{
public:
std::vector<int> modDane(std::string& filePath, int mode);
};
#endif
