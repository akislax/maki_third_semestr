#include "second_task.h"
#include "array_template.h"
#include <string>

using namespace std;
using json = nlohmann::json;

const double a = 0.6180339887;
int hashkey(const string& key)
{
    unsigned long h = 0;
    for (char c : key) h = h * 31 + c;
    return h % 100;
}

int hashkey(int key)
{
    double x = key * a;
    double sda = x - int(x);
    if (sda < 0)
    {
        sda +=1;
    }
    int h = int (100 * sda);
    return h;
}

template <typename T>
int hashTable<T>::hashFunc(const T& key)
{
    int beta = hashkey(key) % SIZE;
    return beta;
}

template <typename T>
bool hashTable<T>::search(const T& key)
{
    int index = hashFunc(key);
    Node<T>* current = table[index];
    while(current != nullptr)
    {
        if (current -> value == key)
        {
            return true;
        }
        current = current -> next;
    }

    return false;
}

template <typename T>
void hashTable<T>::add(const T& key)
{
    if (search(key)) return;
    int index = hashFunc(key);
    Node<T>* new_node = new Node<T>{key, table[index]};
    table[index] = new_node;
}

template <typename T>
void hashTable<T>::remove(const T& key)
{
    int index = hashFunc(key);
    Node<T>* current = table[index];
    Node<T>* prev = nullptr;

    while (current != nullptr)
    {
        if (current->value == key)
        {
            if (prev == nullptr)
                table[index] = current->next;  
            else
                prev->next = current->next;     

            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

template <typename T>
hashTable<T>::~hashTable()
{
    for (int i = 0; i < SIZE; i++)
    {
        Node<T>* current = table[i];
        while (current != nullptr)
        {
            Node<T>* tmp = current;
            current = current->next;
            delete tmp;
        }
    }
}
template <typename T>
void hashTable<T>::loadFromJson(const json& arr)
{
    for (auto& el : arr)
    {
        add(el.get<T>());
    }
}

template <typename T>
json hashTable<T>::saveToJson()
{
    json arr = json::array();
    for (int i = 0; i < SIZE; i++)
    {
        Node<T>* current = table[i];
        while (current != nullptr)
        {
            arr.push_back(current->value);
            current = current->next;
        }
    }
    return arr;
}

template <typename T>
void hashTable<T>::toArray(Array<T>& arr)
{
    for (int i = 0; i < SIZE; i++)
    {
        Node<T>* current = table[i];
        while (current != nullptr)
        {
            pushBack(&arr, current -> value);
            current = current -> next;
        }
    }
}


template class hashTable<int>;
template class hashTable<string>;