#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>

//
//dosc wazne jest to ze klasa zaklada pewien format kodowania, wiec jesli plik nie jest zakodowany w tym formacie to moze byc problem z odkodowaniem
//naglowek - drzewo Huffmana, potem zakodowane dane
class HuffmanEncoder {
public:
    HuffmanEncoder(const std::string& filename);
    ~HuffmanEncoder();
    void printCodes();
    //note: jesli outputFilePath istnieje, to zostanie nadpisany
    static void decompressFile(const std::string& inputFilePath, const std::string& outputFilePath); //kopuje z pliku do pliku (plik zakodowany -> plik odkodowany)
	//note: poniewaz jest to funkcja statyczna, nie musimy inicjalizowac obiektu, uzywamy:  HuffmanEncoder::decompressFile(compressedFile, uncompressedFile);
	void compressToFile(const std::string& outputFilePath); //kopiuje z pliku do pliku (plik nie kodowany -> plik zakodowany)
	//long long getOriginalCharCount() const { return originalCharCount; }
    void buildTree();

private: 
    struct Node;

    struct Compare {
        bool operator()(Node* a, Node* b);
    };

    Node* root;
    std::map<char, std::string> huffmanCodes;
    std::map<char, int> freq;
    std::string inputFilePath;
    //long long originalCharCount;
	HuffmanEncoder(const std::map<char, int>& frequencies); //jest wykorzystywany tylko w decompressFile do trzymania drzewa tymczasowego do odkodowywania (zasadniczo budujemy napjierw drzewo, potem plik)
    void printCodesRecursive(Node* node, const std::string& prefix);
    void destroyTree(Node* node);
    void generateCodes(Node* node, const std::string& prefix);
};
#endif
