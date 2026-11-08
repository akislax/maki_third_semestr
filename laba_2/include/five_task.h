#pragma once
#include <string>
using namespace std;

struct HuffNode
{
    char symbol;
    int weight;
    HuffNode* left;
    HuffNode* right;
};

class HuffTree
{
    private:
    string codes[256]; //это хранение 0 и 1 для дерева
    HuffNode* root = nullptr;
    void buildCodes(HuffNode* node, string code);
    void freeTree(HuffNode* node);

    public:
    void buildTree(const string& text);
    string encode(const string& text);
    string decode(const string& bits);
    void printTable();
    ~HuffTree();
    
};