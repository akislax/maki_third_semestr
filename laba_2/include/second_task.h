#pragma once
#include "array_template.h"
#include <nlohmann/json.hpp>

template <typename T>
struct Node
{
    T value;
    Node<T>* next; 
};

template <typename T>
class hashTable
{
    private:
        static const int SIZE = 100;
        Node<T>* table[SIZE] = {};
        int hashFunc(const T& key);

    public:
        void add(const T& key);
        void remove(const T& key);
        bool search(const T& key);
        void loadFromJson(const nlohmann::json& arr);
        nlohmann::json saveToJson();
        void toArray(Array<T>& arr);
        ~hashTable();
};
