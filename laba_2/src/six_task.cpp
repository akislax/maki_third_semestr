#include "six_task.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <ctime>
using namespace std;



int foldSum(int key)
{
    string s = to_string(abs(key));
    int sum = 0;
    for (int i = 0; i < s.size(); i += 3)
    {
        sum += stoi(s.substr(i,3));
    }
    return sum;
}

int hashTableSix::hashKeySix(const int& key)
{
    return foldSum(key) % SIZE;
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

bool hashTableSix::search(const int& key)
{
    int index = hashKeySix(key);
    NodeSix* current = table[index];
    while (current != nullptr)
    {
        if (current -> value == key)
        {
            return true;
        }
        current = current -> next;
    }
    return false;

}

void hashTableSix::add(const int& key)
{
    if (search(key)) return;
    int index = hashKeySix(key);
    NodeSix* new_node = new NodeSix{key, table[index]};
    table[index] = new_node;
}

void hashTableSix::remove(const int& key)
{
    int index = hashKeySix(key);

    NodeSix* current = table[index];
    NodeSix* prev = nullptr;

    while (current != nullptr)
    {
        if (current->value == key)
        {
            if (prev == nullptr)
            {
                table[index] = current -> next;
            }
            else
            {
                prev -> next = current -> next;
            }
            delete current;
            return;
        }

        prev = current;
        current = current -> next;
    }
}

Kukushka::Kukushka()
{
    size = 11;
    t1 = new int[size];
    t2 = new int[size];
    used1 = new bool[size]();   //каждая ячейка будет иметь значени false что пусто тоже таблица
    used2 = new bool[size]();
}

Kukushka::~Kukushka()
{
    delete[] t1;
    delete[] t2;
    delete[] used1;
    delete[] used2;
}

void Kukushka::moreplace()
{
    int oldSize = size;
    int* old1 = t1;
    int* old2 = t2;
    bool* oldUsed1 = used1;
    bool* oldUsed2 = used2;

    size = size * 2;
    t1 = new int[size];
    t2 = new int[size];
    used1 = new bool[size]();
    used2 = new bool[size]();

    for (int i = 0; i < oldSize; i++)
    {
        if (oldUsed1[i]) insert (old1[i]); //insert работает с новыми t1 поэтому
        if (oldUsed2[i]) insert (old2[i]);
    }
     
    delete[] old1;
    delete[] old2;
    delete[] oldUsed1;
    delete[] oldUsed2;

}

int Kukushka::hash1(const int& key)
{
    return foldSum(key) % size;
}

int Kukushka::hash2(const int& key)
{
    int h = key % size;
    if (h < 0)
    {
        h += size;
    }
    return h;
}

bool Kukushka::search(const int& key)
{
    int i1 = hash1(key);
    if (used1[i1] && t1[i1] == key)
    {
        return true;
    }

    int i2 = hash2(key);
    if (used2[i2] && t2[i2] == key)
    {
        return true;
    }
    return false;

}

void Kukushka::insert(const int& key)
{
    if (search(key)) return;

    int cur = key;

    for(int tempt = 0; tempt < MAXTEMPT; tempt++)
    {
        int i1 = hash1(cur);

        if (used1[i1] == false)
        {
            t1[i1] = cur;
            used1[i1] = true;
            return;
        }

        int tmp = t1[i1];
        t1[i1] = cur;
        cur = tmp;

        int i2 = hash2(cur);
        if (used2[i2] == false)
        {
            t2[i2] = cur;
            used2[i2] = true;
            return;
        }
        tmp = t2[i2];
        t2[i2] = cur;
        cur = tmp;
    }
    moreplace();
    insert(cur);
}

void Kukushka::remove(const int& key)
{
    int i1 = hash1(key);
    if (used1[i1] && t1[i1] == key)
    {
        used1[i1] = false;
        return;
    }
    int i2 = hash2(key);
    if (used2[i2] && t2[i2] == key)
    {
        used2[i2] = false;
        return;
    }
}
void printFold(const int& key)
{
    string s = to_string(abs(key));
    int sum = 0;
    for (int i = 0; i < s.size(); i += 3)
    {
        string part = s.substr(i, 3);
        if (i > 0) cout << "+";
        cout << part;
        sum += stoi(part);
    }
    cout << " = " << sum << endl;
}
void compareHash()
{
    int sizes[] = {1000, 10000, 50000};
    srand(time(nullptr));

    cout << "Элементов | Цепочки: вставка, поиск (мс) | Кукушка: вставка, поиск (мс)" << endl;

    for (int n : sizes)
    {
        // случайные числа
        int* keys = new int[n];
        for (int i = 0; i < n; i++)
        {
            keys[i] = rand() % 1000000;
        }

        hashTableSix chain;
        Kukushka cuckoo;

        // цепочки: вставка
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++) chain.add(keys[i]);
        auto end = chrono::high_resolution_clock::now();
        double chainInsert = chrono::duration<double, milli>(end - start).count();

        // цепочки: поиск
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++) chain.search(keys[i]);
        end = chrono::high_resolution_clock::now();
        double chainSearch = chrono::duration<double, milli>(end - start).count();

        // кукушка: вставка
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++) cuckoo.insert(keys[i]);
        end = chrono::high_resolution_clock::now();
        double cuckooInsert = chrono::duration<double, milli>(end - start).count();

        // кукушка: поиск
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++) cuckoo.search(keys[i]);
        end = chrono::high_resolution_clock::now();
        double cuckooSearch = chrono::duration<double, milli>(end - start).count();

        cout << n << " | " << chainInsert << ", " << chainSearch
             << " | " << cuckooInsert << ", " << cuckooSearch << endl;

        delete[] keys;
    }
}