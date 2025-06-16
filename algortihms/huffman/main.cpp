#include "huffman2.h"
#include <iostream>

int main() {
    //note: nie robilem jakis glebokich testow, ale powinno dzialac
    HuffmanEncoder encoder("dane.txt");

    std::cout << "Huffman Codes:\n";
    encoder.printCodes();
    encoder.compressToFile("out.bin");
    HuffmanEncoder::decompressFile("out.bin", "dane2.txt");

    return 0;
}
