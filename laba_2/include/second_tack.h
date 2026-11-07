#pragma once

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
        void search(const T& key);
        ~hashTable();
};
