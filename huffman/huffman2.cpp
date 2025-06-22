#include "huffman2.h"

    struct HuffmanEncoder::Node {
    public:
        char chr;
        int data;
        Node* left, * right;

        Node(char c, int x);
        Node(int f, Node* l, Node* r);
    };

    HuffmanEncoder::HuffmanEncoder(const std::string& filename) : inputFilePath(filename), root(nullptr) {
        std::ifstream input(filename);
        if (!input.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return;
        }

        char c;
        while (input.get(c)) {
            if (this->freq.count(c) == 0) {
                this->freq.insert(std::make_pair(c, 1));
            }
            else {
                this->freq[c] += 1;
            }
        }

        if (this->freq.empty()) {
            return;
        }
        

        buildTree();
        generateCodes(root, "");
    }
    HuffmanEncoder::HuffmanEncoder(const std::map<char, int>& frequencies) : root(nullptr), freq(frequencies) {};
    HuffmanEncoder::~HuffmanEncoder() {
        destroyTree(root);
    }
    void HuffmanEncoder::printCodes() {
        if (root == nullptr) {
            std::cout << "Root=nullptr" << std::endl;
            return;
        }
        printCodesRecursive(root, "");
    }
    HuffmanEncoder::Node::Node(char c, int x) : chr(c), data(x), left(nullptr), right(nullptr) {}

    HuffmanEncoder::Node::Node(int f, Node* l, Node* r) : chr('\0'), data(f), left(l), right(r) {}

    bool HuffmanEncoder::Compare::operator()(Node* a, Node* b) {
        return a->data > b->data;
    }
    void HuffmanEncoder::compressToFile(const std::string& outputFilePath) {
        if (this->root == nullptr) {
            std::cerr << "Error: Huffman tree is not built. Cannot compress." << std::endl;
            return;
        }

        std::ofstream outputFile(outputFilePath, std::ios::binary);
        if (!outputFile.is_open()) {
            std::cerr << "Error: Cannot open output file: " << outputFilePath << std::endl;
            return;
        }
        std::ifstream inputFile(this->inputFilePath);
        if (!inputFile.is_open()) {
            std::cerr << "Error: Could not open source file: " << this->inputFilePath << std::endl;
            return;
        }

		//najpierw info o drzewie, potem dane (rozmiar mapy, mapa, dane)
        uint16_t mapSize = freq.size();
        outputFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

        for (const auto& pair : freq) {
            outputFile.write(&pair.first, sizeof(pair.first));
            outputFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
        }

		// wpisywanie danych skompresowanych
        char c;
        unsigned char buffer = 0;
        int bits_in_buffer = 0;

		//bierze plik wejsciowy, przypisuje kody Huffmana do znaków, wpisuje do bufora i bufor do pliku wyjsciowego (jak bufor bedzie mial 8 bitow, wtedy wpisuje)
        while (inputFile.get(c)) {
            std::string code = huffmanCodes.at(c);
            for (char bit : code) {
				buffer = (buffer << 1) | (bit - '0'); // '0' to 48, '1' to 49, wiec odejmujemy 48, zeby dostac 0..0 lub 0..1; OR zasadniczo doczepia ten bit do bufora
                bits_in_buffer++;
                if (bits_in_buffer == 8) {
                    outputFile.write(reinterpret_cast<const char*>(&buffer), sizeof(buffer));
                    buffer = 0;
                    bits_in_buffer = 0;
                }
            }
        }

		// koncowka, moze zostac bufor z niekompletnym bajtem (nie powinno to stwarzac problemow, bo dekoder liczy ile jest charow w pliku i przestanie czytac, ignorujac zera)
        if (bits_in_buffer > 0) {
            buffer <<= (8 - bits_in_buffer);
            outputFile.write(reinterpret_cast<const char*>(&buffer), sizeof(buffer));
        }
        outputFile.close();
        inputFile.close();
    }

    void HuffmanEncoder::printCodesRecursive(Node* node, const std::string& prefix) {
        if (node == nullptr) {
            return;
        }
        if (node->chr != '\0') {
            std::cout << (char)node->chr << ": " << prefix << "\n";
        }
        printCodesRecursive(node->left, prefix + "0");
        printCodesRecursive(node->right, prefix + "1");
    }

    void HuffmanEncoder::destroyTree(Node* node) {
        if (node == nullptr) {
            return;
        }
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
    void HuffmanEncoder::decompressFile(const std::string& inputFilePath, const std::string& outputFilePath) {
        std::ifstream inputFile(inputFilePath, std::ios::binary);
        if (!inputFile.is_open()) {
            std::cerr << "Error: Cannot open input file: " << inputFilePath << std::endl;
            return;
        }
        std::ofstream outputFile(outputFilePath);
    
        if (!outputFile.is_open()) {
            std::cerr << "Error: Cannot open output file: " << outputFilePath << std::endl;

            return;
        }

		// czytanie nag³ówka z pliku wejœciowego i odbudowanie drzewa Huffmana
        std::map<char, int> freqs; //note: to inna zmienna niz freq w samej klasie
        long long totalChars = 0;

		// czytamy rozmiar mapy znaków (uint16_t do tego zeby rozmiar w pamieci byl znany, moze byc ogranicznikiem rozmiaru/roznorodnosci pliku!)
        uint16_t mapSize;
        inputFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

        // czytamy mape
        for (int i = 0; i < mapSize; i++) {
            char character;
            int frequency;
            inputFile.read(&character, sizeof(character));
            inputFile.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));
            freqs[character] = frequency;
			totalChars += frequency; //zliczamy ile znaków jest w pliku, ¿eby wiedzieæ kiedy skoñczyæ dekodowanie (bo mo¿e byæ bufor z niekompletnym bajtem na koñcu,
            // no i sam system mozne nam "dopelnic"/zaalokowac wiecej pamieci niz potrzebujemy itp, a eof gwarancji nam nie daje)
        }

		// Musimy zbudowaæ drzewo Huffmana na podstawie odczytanej mapy czêstotliwoœci
        HuffmanEncoder decoder(freqs);
        decoder.buildTree();
        if (decoder.root == nullptr) {
            std::cerr << "Error: Failed to rebuild Huffman tree from file header." << std::endl;
            return;
        }

        // wlasciwe dane
        Node* currentNode = decoder.root;
        char byte;
        long long decodedCount = 0;
        while (inputFile.get(byte) && decodedCount < totalChars) {
            for (int i = 7; i >= 0; i--) {
                if (decodedCount >= totalChars) break;

                bool isBitSet = (byte >> i) & 1;
                currentNode = isBitSet ? currentNode->right : currentNode->left;

                if (currentNode != nullptr) {
                    if (currentNode->left == nullptr && currentNode->right == nullptr) {
                        outputFile << currentNode->chr;
                        currentNode = decoder.root;
                        decodedCount++;
                    }
                }
                else {
                    std::cerr << "Warning: Invalid bit sequence encountered during decompression." << std::endl;
                    currentNode = decoder.root;
                    break;
                }
            }
        }
        outputFile.close();
        inputFile.close();

    }
    void HuffmanEncoder::buildTree() {
        std::priority_queue<Node*, std::vector<Node*>, Compare> q;
        for (auto& p : this->freq) {
            q.push(new Node(p.first, p.second));
        }
        if (q.size() == 1) { //Edge case: jeden znak w pliku
            Node* leaf = q.top();
            this->root = new Node(leaf->data, leaf, nullptr);
            return;
        }

        while (q.size() > 1) {
            Node* left = q.top(); q.pop();
            Node* right = q.top(); q.pop();

            Node* parent = new Node(left->data + right->data, left, right);
            q.push(parent);
        }

        this->root = q.top();
    }
    void HuffmanEncoder::generateCodes(Node* node, const std::string& prefix) {
        if (node == nullptr) return;
        if (node->left == nullptr && node->right == nullptr) {
            if (node->chr != '\0') huffmanCodes[node->chr] = prefix.empty() ? "0" : prefix;
        }
        generateCodes(node->left, prefix + "0");
        generateCodes(node->right, prefix + "1");
    }