#pragma once
#include <string>
#include <cstddef>

struct Node
{
    std::string value;
    std::string key;
    Node* next = nullptr;
};

class HashTable
{
    public:
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable(size_t start_Capacity = 20);
    ~HashTable();
    void add(const std::string& key, const std::string& value);
    bool remove (const std::string& key);
    bool search (const std::string& key) const;
    std::string get(const std::string& key) const;
    Node** getTable() const;
    size_t getCapacity() const;

    private:
    Node** table;
    size_t capacity;
    size_t count;

    size_t hashFunc(const std::string& key) const; 
    void rehash();
};