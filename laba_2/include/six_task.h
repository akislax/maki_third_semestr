#pragma once
#include <string>
using namespace std;

struct NodeSix
{
    string key;
    string value;
    NodeSix* next;
};

class hashTableSix
{
private:
    static const int SIZE = 100;
    NodeSix* table[SIZE] = {};
    int hashKeySix(const string& key);

public:
    void add(const string& key, const string& value);
    void remove(const string& key);
    string search(const string& key);
    ~hashTableSix();
};

class Kukushka
{
private:
    static const int MAXTEMPT = 50;
    int size;
    string* keys1;
    string* vals1;
    string* keys2;
    string* vals2;
    bool* used1;
    bool* used2;

    int hash1(const string& key);
    int hash2(const string& key);
    void moreplace();

public:
    Kukushka();
    ~Kukushka();
    void insert(const string& key, const string& value);
    string search(const string& key);
    void remove(const string& key);
};

int foldSum(const string& key);
void printFold(const string& key);
void compareHash();