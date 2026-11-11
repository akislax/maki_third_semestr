#include "six_task.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <ctime>
using namespace std;

int foldSum(const string& key)
{
    int sum = 0;
    for (int i = 0; i < key.size(); i += 3)
    {
        string part = key.substr(i, 3);

        int num = 0;
        for (char c : part)
        {
            num = num * 10 + (c - '0');
        }
        sum += num;
    }
    return abs(sum);
}

int hashTableSix::hashKeySix(const string& key)
{
    return foldSum(key) % SIZE;
}

string hashTableSix::search(const string& key)
{
    int index = hashKeySix(key);
    NodeSix* current = table[index];
    while (current != nullptr)
    {
        if (current->key == key)
        {
            return current->value;
        }
        current = current->next;
    }
    return "";
}

void hashTableSix::add(const string& key, const string& value)
{
    int index = hashKeySix(key);

    NodeSix* current = table[index];
    while (current != nullptr)
    {
        if (current->key == key)
        {
            current->value = value;
            return;
        }
        current = current->next;
    }

    NodeSix* new_node = new NodeSix{key, value, table[index]};
    table[index] = new_node;
}

void hashTableSix::remove(const string& key)
{
    int index = hashKeySix(key);

    NodeSix* current = table[index];
    NodeSix* prev = nullptr;

    while (current != nullptr)
    {
        if (current->key == key)
        {
            if (prev == nullptr)
            {
                table[index] = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

hashTableSix::~hashTableSix()
{
    for (int i = 0; i < SIZE; i++)
    {
        NodeSix* current = table[i];
        while (current != nullptr)
        {
            NodeSix* tmp = current;
            current = current->next;
            delete tmp;
        }
    }
}

Kukushka::Kukushka()
{
    size = 11;
    keys1 = new string[size];
    vals1 = new string[size];
    keys2 = new string[size];
    vals2 = new string[size];
    used1 = new bool[size]();   //каждая ячейка будет иметь значени false что пусто тоже таблица
    used2 = new bool[size]();
}

Kukushka::~Kukushka()
{
    delete[] keys1;
    delete[] vals1;
    delete[] keys2;
    delete[] vals2;
    delete[] used1;
    delete[] used2;
}

int Kukushka::hash1(const string& key)
{
    return foldSum(key) % size;
}

int Kukushka::hash2(const string& key)
{
    unsigned long h = 0;
    for (char c : key)
    {
        h = h * 31 + (unsigned char)c;
    }
    return h % size;
}

void Kukushka::moreplace()
{
    int oldSize = size;
    string* oldKeys1 = keys1;
    string* oldVals1 = vals1;
    string* oldKeys2 = keys2;
    string* oldVals2 = vals2;
    bool* oldUsed1 = used1;
    bool* oldUsed2 = used2;

    size = size * 2;
    keys1 = new string[size];
    vals1 = new string[size];
    keys2 = new string[size];
    vals2 = new string[size];
    used1 = new bool[size]();
    used2 = new bool[size]();

    for (int i = 0; i < oldSize; i++)
    {
        if (oldUsed1[i]) insert(oldKeys1[i], oldVals1[i]); //insert работает с новыми t1 поэтому
        if (oldUsed2[i]) insert(oldKeys2[i], oldVals2[i]);
    }

    delete[] oldKeys1;
    delete[] oldVals1;
    delete[] oldKeys2;
    delete[] oldVals2;
    delete[] oldUsed1;
    delete[] oldUsed2;
}

string Kukushka::search(const string& key)
{
    int i1 = hash1(key);
    if (used1[i1] && keys1[i1] == key)
    {
        return vals1[i1];
    }

    int i2 = hash2(key);
    if (used2[i2] && keys2[i2] == key)
    {
        return vals2[i2];
    }
    return "";
}

void Kukushka::insert(const string& key, const string& value)
{
    int i1 = hash1(key);
    if (used1[i1] && keys1[i1] == key)
    {
        vals1[i1] = value;
        return;
    }
    int i2 = hash2(key);
    if (used2[i2] && keys2[i2] == key)
    {
        vals2[i2] = value;
        return;
    }

    string curKey = key;
    string curVal = value;

    for (int tempt = 0; tempt < MAXTEMPT; tempt++)
    {
        i1 = hash1(curKey);
        if (used1[i1] == false)
        {
            keys1[i1] = curKey;
            vals1[i1] = curVal;
            used1[i1] = true;
            return;
        }
        swap(keys1[i1], curKey);
        swap(vals1[i1], curVal);

        i2 = hash2(curKey);
        if (used2[i2] == false)
        {
            keys2[i2] = curKey;
            vals2[i2] = curVal;
            used2[i2] = true;
            return;
        }
        swap(keys2[i2], curKey);
        swap(vals2[i2], curVal);
    }

    moreplace();
    insert(curKey, curVal);
}

void Kukushka::remove(const string& key)
{
    int i1 = hash1(key);
    if (used1[i1] && keys1[i1] == key)
    {
        used1[i1] = false;
        return;
    }
    int i2 = hash2(key);
    if (used2[i2] && keys2[i2] == key)
    {
        used2[i2] = false;
    }
}

void printFold(const string& key)
{
    int sum = 0;
    for (int i = 0; i < key.size(); i += 3)
    {
        string part = key.substr(i, 3);
        if (i > 0) cout << "+";
        cout << part;

        int num = 0;
        for (char c : part)
        {
            num = num * 10 + (c - '0');
        }
        sum += num;
    }
    cout << " = " << abs(sum) << endl;
}

void compareHash()
{
    int sizes[] = {1000, 10000, 50000};
    srand(time(nullptr));

    cout << "Элементов | Цепочки: вставка, поиск (мс) | Кукушка: вставка, поиск (мс)" << endl;

    for (int n : sizes)
    {
        string* keys = new string[n];
        string* values = new string[n];
        for (int i = 0; i < n; i++)
        {
            keys[i] = to_string(rand() % 1000000);
            values[i] = "value" + to_string(i);
        }

        hashTableSix chain;
        Kukushka cuckoo;

        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++) chain.add(keys[i], values[i]);
        auto end = chrono::high_resolution_clock::now();
        double chainInsert = chrono::duration<double, milli>(end - start).count();

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++) chain.search(keys[i]);
        end = chrono::high_resolution_clock::now();
        double chainSearch = chrono::duration<double, milli>(end - start).count();

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++) cuckoo.insert(keys[i], values[i]);
        end = chrono::high_resolution_clock::now();
        double cuckooInsert = chrono::duration<double, milli>(end - start).count();

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++) cuckoo.search(keys[i]);
        end = chrono::high_resolution_clock::now();
        double cuckooSearch = chrono::duration<double, milli>(end - start).count();

        cout << n << " | " << chainInsert << ", " << chainSearch
             << " | " << cuckooInsert << ", " << cuckooSearch << endl;

        delete[] keys;
        delete[] values;
    }
}