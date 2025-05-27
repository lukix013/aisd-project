#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>

class Node{
    public:
        int chr;
        int data;
        Node *left, *right;
        Node(char c ,int x){
            chr = c;
            data = x;
            left = nullptr;
            right = nullptr;
        }

        Node(int f, Node* l, Node* r){
            chr = '\0';
            data = f;
            left = l;
            right = r;
        }
};

struct Compare{
    bool operator()(Node* a, Node*b){
        return a->data > b->data;
    }
};

void printHuffman(Node* root, const std::string &prefix = ""){
    if (root == nullptr) {
        return;
    }

    if (root->chr != '\0') {
        std::cout << (char) root->chr << ": " << prefix << "\n";
    }

    printHuffman(root->left, prefix + "0");
    printHuffman(root->right, prefix + "1");
}

int main(void){

    std::map<char, int> freq;
    std::priority_queue<Node*, std::vector<Node*>, Compare> q;

    std::ifstream input("dane.txt");
    char c;

    while (!input.eof()) {
        c = input.get();
        if (c >= 'a' && c <= 'z') {
            if (freq.count(c) == 0) {
            freq.insert(std::make_pair(c, 1));
        } else {
            freq[c] += 1;
        }
        }
    }

    for (auto &p : freq) {
        q.push(new Node(p.first, p.second));
    }

    while (q.size() > 1) {
        Node* left = q.top(); q.pop();
        Node* right = q.top(); q.pop();

        Node* parent = new Node(left->data + right->data, left, right);
        q.push(parent);
    }

    Node* root = q.top();

    printHuffman(root);
    
    return 0;
}