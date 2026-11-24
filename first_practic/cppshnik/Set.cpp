#include "Set.h"

Set::Set(size_t start_Capacity)
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

    table = new NodeSet*[capacity];

    for (size_t i = 0; i < capacity; i++)
    {
        table[i] = nullptr;
    }
}

Set::~Set()
{
    for (size_t i = 0; i < capacity; i++)
    {
        NodeSet* current = table[i];

        while (current != nullptr)
        {
            NodeSet* tmp = current;
            current = current->next;
            delete tmp;
        }
    }

    delete[] table;
}

size_t Set::hashFunc(const std::string& value) const
{
    size_t h = 0;

    for (unsigned char c : value)
        h = h * 31 + c;

    return h % capacity;
}

void Set::add(const std::string& value)
{
    size_t abc = hashFunc(value);

    NodeSet* current = table[abc];

    while (current != nullptr)
    {
        if (current->value == value)
        {
            return;
        }

        current = current->next;
    }

    NodeSet* newNode = new NodeSet;

    newNode->value = value;

    newNode->next = table[abc];
    table[abc] = newNode;

    count++;

    if (static_cast<double>(count) / capacity > 0.75)
    {
        rehash();
    }
}

bool Set::search(const std::string& value) const
{
    size_t abc = hashFunc(value);

    NodeSet* current = table[abc];

    while (current != nullptr)
    {
        if (current->value == value)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

bool Set::remove(const std::string& value)
{
    size_t abc = hashFunc(value);

    NodeSet* current = table[abc];
    NodeSet* tmp = nullptr;

    while (current != nullptr)
    {
        if (current->value == value)
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

void Set::rehash()
{
    size_t oldCapacity = capacity;
    NodeSet** oldTable = table;

    capacity *= 2;
    table = new NodeSet*[capacity];

    for (size_t i = 0; i < capacity; i++)
    {
        table[i] = nullptr;
    }

    for (size_t i = 0; i < oldCapacity; i++)
    {
        NodeSet* current = oldTable[i];

        while (current != nullptr)
        {
            NodeSet* next = current->next;

            size_t cba = hashFunc(current->value);

            current->next = table[cba];
            table[cba] = current;

            current = next;
        }
    }

    delete[] oldTable;
}

NodeSet** Set::getTable() const
{
    return table;
}

size_t Set::getCapacity() const
{
    return capacity;
}