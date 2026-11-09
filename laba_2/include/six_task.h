#pragma once
using namespace std;


struct NodeSix
{
    int value;
    NodeSix* next; 
};

class hashTableSix
{
    private:
        static const int SIZE = 100;
        NodeSix* table[SIZE] = {};
        int hashKeySix(const int& key);

    public:
        void add(const int& key);
        void remove(const int& key);
        bool search(const int& key);
        ~hashTableSix();
};

class Kukushka
{
    private:
    
    static const int MAXTEMPT = 50;
    int size;
    int* t1;
    int* t2;
    bool* used1;
    bool* used2;
    
    int hash1(const int& key);
    int hash2(const int& key);
    void moreplace();

    public:
    Kukushka();
    ~Kukushka();
    void insert(const int& key);
    bool search(const int& key);
    void remove(const int& key);
};
void printFold(const int& key);
void compareHash();