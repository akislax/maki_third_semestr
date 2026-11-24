#pragma once
#include <string>
#include <cstddef>

struct NodeSet
{
    std::string value;
    NodeSet* next = nullptr;
};

class Set
{
    public:
    Set(const Set&) = delete;
    Set& operator=(const Set&) = delete;
    Set(size_t start_Capacity = 20);
    ~Set();
    void add(const std::string& value);
    bool remove (const std::string& value);
    bool search (const std::string& value) const;
    NodeSet** getTable() const;
    size_t getCapacity() const;

    private:
    NodeSet** table;
    size_t capacity;
    size_t count;

    size_t hashFunc(const std::string& value) const; 
    void rehash();
};