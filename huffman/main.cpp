#include "huffman2.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string input;
    std::string output;
    char mode;
    if(argc == 1) {
        std::cout << "Pamietaj o rzszerzeniach plikow. Jesli plik znajduje sie w tym samym folderze co ten program, wystarczy nazwa pliku." << std::endl;
        std::cout << "Czy chcesz komperesowac plik (k), czy odkompresowac (d): " << std::endl;
        std::cin>>mode;
        std::cout << "Podaj sciezke pliku wejsciowego: " << std::endl;
        std::cin >> input;
        std::cout << "Podaj sciezke pliku wyjsciowego: " << std::endl;
		std::cin >> output;
        if(mode == 'k') {
            HuffmanEncoder encoder(input);
            encoder.compressToFile(output);
            return 0;
        } else if(mode == 'd') {
            HuffmanEncoder::decompressFile(input, output);
            return 0;
        } else {
            std::cerr << "Nieznany tryb. Uzyj 'k' do kompresji lub 'd' do dekompresji." << std::endl;
            return 1;
        }
	}
    //dane podane przez argumenty
    else if(argc == 4) {
        input = argv[1];
        output = argv[2];
        mode = argv[3][0];
        if(mode == 'k') {
            HuffmanEncoder encoder(input);
            encoder.compressToFile(output);
        } else if(mode == 'd') {
            HuffmanEncoder::decompressFile(input, output);
        } else {
            std::cerr << "Nieznany tryb. Uzyj 'k' do kompresji lub 'd' do dekompresji." << std::endl;
            return 1;
        }
        return 0;
	}
    else {
          std::cerr << "Niewlasciwa liczba argumentow. Uzyj: ./huffman <plik_wejsciowy> <plik_wyjsciowy> tryb(k/d), albo bez argumentow" << std::endl;
          return 1;
    }
	
    /* wypisanie kodów, do spr
    std::cout << "Huffman Codes:\n";
    encoder.printCodes(); */
    return 0;
}