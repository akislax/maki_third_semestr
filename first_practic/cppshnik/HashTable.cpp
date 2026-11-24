#include "HashTable.h"
#include <stdexcept>

HashTable::HashTable(size_t start_Capacity)
{
    if (start_Capacity == 0)
    {
        capacity = 20;
    }
    else
    {
        capacity = start_Capacity;
    }

    count = 0;

    table = new Node *[capacity]; // этой строчкой мы создали таблицу из указателей, которая хранит указатель на ноду. а нью нодой добавляем элемент
    for (size_t i = 0; i < capacity; i++)
    {
        table[i] = nullptr;
    }
}

HashTable::~HashTable()
{
    for (size_t i = 0; i < capacity; i++)
    {
        Node *current = table[i];

        while (current != nullptr)
        {
            Node *tmp = current;
            current = current->next;
            delete tmp;
        }
    }
    delete[] table;
}

size_t HashTable::hashFunc(const std::string &key) const
{
    size_t h = 0;

    for (char c : key)
        h = h * 31 + c;
    return h % capacity;
}

void HashTable::add(const std::string &key, const std::string &value)
{
    size_t abc = hashFunc(key);

    Node *current = table[abc];

    while (current != nullptr)
    {
        if (current->key == key)
        {
            current->value = value;
            return;
        }

        current = current->next;
    }

    Node *newNode = new Node;

    newNode->key = key;
    newNode->value = value;

    newNode->next = table[abc];
    table[abc] = newNode;

    count++;

    if (static_cast<double>(count) / capacity > 0.75)
    {
        rehash();
    }
}

bool HashTable::search(const std::string &key) const
{
    size_t abc = hashFunc(key);

    Node *current = table[abc];

    while (current != nullptr)
    {
        if (current->key == key)
        {
            return true;
        }
        current = current->next;
    }

    return false;
}

std::string HashTable::get(const std::string &key) const
{
    size_t abc = hashFunc(key);

    Node *current = table[abc];

    while (current != nullptr)
    {
        if (current->key == key)
        {
            return current->value;
        }
        current = current->next;
    }

    throw std::runtime_error("Ключ не найден");
}

bool HashTable::remove(const std::string &key)
{
    size_t abc = hashFunc(key);

    Node *current = table[abc];

    Node *tmp = nullptr;

    while (current != nullptr)
    {
        if (current->key == key)
        {
            if (tmp == nullptr)
            {
                table[abc] = current->next;
            }
            else
            {
                tmp->next = current->next;
            }
            delete current;
            count--;
            return true;
        }
        tmp = current;
        current = current->next;
    }
    return false;
}

void HashTable::rehash()
{
    size_t oldCapacity = capacity;
    Node **oldTable = table;

    capacity *= 2;
    table = new Node *[capacity];

    for (size_t i = 0; i < capacity; i++)
    {
        table[i] = nullptr;
    }

    for (size_t i = 0; i < oldCapacity; i++)
    {
        Node *current = oldTable[i];

        while (current != nullptr)
        {
            Node *next = current->next;

            size_t cba = hashFunc(current->key);

            current->next = table[cba];

            table[cba] = current;

            current = next;
        }
    }
    delete[] oldTable;
}
Node** HashTable::getTable() const
{
    return table;
}

size_t HashTable::getCapacity() const
{
    return capacity;
}