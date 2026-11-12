#include "hashTable.h"
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;



int foldSum(int key)
{
    string s = to_string(abs(key));
    int sum = 0;
    for (size_t i = 0; i < s.size(); i += 3)
    {
        sum += stoi(s.substr(i,3));
    }
    return sum;
}

int hashTable::hashKey(const int& key) const
{
    return foldSum(key) % SIZE;
}

hashTable::~hashTable()
{
    for (int i = 0; i < SIZE; i++)
    {
        Node* current = table[i];
        while (current != nullptr)
        {
            Node* tmp = current;
            current = current->next;
            delete tmp;
        }
    }
}

bool hashTable::search(const int& key) const
{
    int index = hashKey(key);
    Node* current = table[index];
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

void hashTable::add(const int& key)
{
    if (search(key)) return;
    int index = hashKey(key);
    Node* new_node = new Node{key, table[index]};
    table[index] = new_node;
}

void hashTable::remove(const int& key)
{
    int index = hashKey(key);

    Node* current = table[index];
    Node* prev = nullptr;

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

void hashTable::print() const
{
    for (int i = 0; i < SIZE; i++)
    {
        if (table[i] == nullptr) continue;   // пустые ячейки пропускаем
        cout << i << ": ";
        for (Node* cur = table[i]; cur != nullptr; cur = cur -> next)
        {
            cout << cur -> value << " -> ";
        }
        cout << "nullptr\n";
    }
}

vector<int> hashTable::getAll() const
{
    vector<int> result;
    for (int i = 0; i < SIZE; i++)                   // по всем ячейкам
    {
        for (Node* cur = table[i]; cur != nullptr; cur = cur -> next)   // по цепочке в ячейке
        {
            result.push_back(cur -> value);
        }
    }
    return result;
}


void handleHash(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "hash")) return;

    hashTable ht;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            ht.add(element.get<int>());
        }
    }

    int value;
    switch (command)
    {
        case Command::HADD:
            ss >> value;
            ht.add(value);
            cout << "-> " << value << "\n";
            break;
        case Command::HDEL:
            ss >> value;
            ht.remove(value);
            break;
        case Command::HFIND:
            ss >> value;
            if (ht.search(value)) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::HPRINT:
            ht.print();
            break;
        default:
            break;
    }

    json data = json::array();
    for (int key : ht.getAll())
    {
        data.push_back(key);
    }
    db[name]["type"] = "hash";
    db[name]["data"] = data;
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

int Kukushka::hash1(const int& key) const
{
    return foldSum(key) % size;
}

int Kukushka::hash2(const int& key) const
{
    int h = key % size;
    if (h < 0)
    {
        h += size;
    }
    return h;
}

bool Kukushka::search(const int& key) const
{
    int i1 = hash1(key);
    if (used1[i1] && t1[i1] == key)
    {
        return true;
    }

    int i2 = hash2(key);
    return used2[i2] && t2[i2] == key;
}

void Kukushka::insert(const int& key)
{
    if (search(key)) return;

    int cur = key;

    for(int tempt = 0; tempt < MAXTEMPT; tempt++)
    {
        int i1 = hash1(cur);

        if (!used1[i1])
        {
            t1[i1] = cur;
            used1[i1] = true;
            return;
        }

        int tmp = t1[i1];
        t1[i1] = cur;
        cur = tmp;

        int i2 = hash2(cur);
        if (!used2[i2])
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

void Kukushka::print() const
{
    cout << "t1: ";
    for (int i = 0; i < size; i++)
    {
        if (used1[i]) cout << "[" << i << "]=" << t1[i] << " ";
    }
    cout << "\nt2: ";
    for (int i = 0; i < size; i++)
    {
        if (used2[i]) cout << "[" << i << "]=" << t2[i] << " ";
    }
    cout << "\n";
}

vector<int> Kukushka::getAll() const
{
    vector<int> result;
    for (int i = 0; i < size; i++)
    {
        if (used1[i]) result.push_back(t1[i]);   // занятые ячейки первой таблицы
    }
    for (int i = 0; i < size; i++)
    {
        if (used2[i]) result.push_back(t2[i]);   // занятые ячейки второй таблицы
    }
    return result;
}

void handleKukushka(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "kukushka")) return;

    Kukushka kt;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            kt.insert(element.get<int>());
        }
    }

    int value;
    switch (command)
    {
        case Command::KADD:
            ss >> value;
            kt.insert(value);
            cout << "-> " << value << "\n";
            break;
        case Command::KDEL:
            ss >> value;
            kt.remove(value);
            break;
        case Command::KFIND:
            ss >> value;
            if (kt.search(value)) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::KPRINT:
            kt.print();
            break;
        default:
            break;
    }

    json data = json::array();
    for (int key : kt.getAll())
    {
        data.push_back(key);
    }
    db[name]["type"] = "kukushka";
    db[name]["data"] = data;
}
// сериализация цепочек: [количество] [ключи]
// ячейку хранить не нужно - при загрузке add сам посчитает хеш
void hashTable::saveBinary(const string& fileName) const
{
    vector<int> keys = getAll();
    ofstream out(fileName, ios::binary);
    writeInt(out, static_cast<int>(keys.size()));
    for (int key : keys)
    {
        writeInt(out, key);
    }
}

void hashTable::loadBinary(const string& fileName)
{
    ifstream in(fileName, ios::binary);
    if (!in.is_open()) return;

    for (int key : getAll()) remove(key);      // очищаем таблицу перед загрузкой
    int count = readInt(in);
    for (int i = 0; i < count; i++)
    {
        add(readInt(in));
    }
}

// сериализация кукушки: так же, [количество] [ключи]
void Kukushka::saveBinary(const string& fileName) const
{
    vector<int> keys = getAll();
    ofstream out(fileName, ios::binary);
    writeInt(out, static_cast<int>(keys.size()));
    for (int key : keys)
    {
        writeInt(out, key);
    }
}

void Kukushka::loadBinary(const string& fileName)
{
    ifstream in(fileName, ios::binary);
    if (!in.is_open()) return;

    for (int key : getAll()) remove(key);      // очищаем таблицу перед загрузкой
    int count = readInt(in);
    for (int i = 0; i < count; i++)
    {
        insert(readInt(in));
    }
}
