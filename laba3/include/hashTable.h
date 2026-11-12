#pragma once
#include "serialize.h"
#include <vector>
#include <vector>
#include "commandsClass.h"
using namespace std;


struct Node
{
    int value;
    Node* next; 
};

class hashTable
{
    private:
        static const int SIZE = 100;
        Node* table[SIZE] = {};
        int hashKey(const int& key) const;

    public:
    void saveBinary(const string &fileName) const;
    void loadBinary(const string &fileName);
        hashTable() = default;
        hashTable(const hashTable&) = delete;
        hashTable& operator=(const hashTable&) = delete;
        void add(const int& key);
        void remove(const int& key);
        bool search(const int& key) const;
        ~hashTable();
        void print() const;
        vector<int> getAll() const;
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
    
    int hash1(const int& key) const;
    int hash2(const int& key) const;
    void moreplace();

    public:
    void saveBinary(const string &fileName) const;
    void loadBinary(const string &fileName);
    Kukushka(const Kukushka&) = delete;
    Kukushka& operator=(const Kukushka&) = delete;
    Kukushka();
    ~Kukushka();
    void insert(const int& key);
    bool search(const int& key) const;
    void remove(const int& key);
    void print() const;
    vector<int> getAll() const;
};

  void handleHash(Command command, json &db, const string &name, stringstream &ss);
  void handleKukushka(Command command, json &db, const string &name, stringstream &ss);