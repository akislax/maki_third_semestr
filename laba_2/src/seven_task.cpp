#include "seven_task.h"
#include "array_template.h"
#include <iostream>
using namespace std;

LRUCache::LRUCache(int cap)
{
    mesto = cap;
    initArray(&keys);
    initArray(&values);
}

LRUCache::~LRUCache()
{
    freeArray(&keys);
    freeArray(&values);
}

int LRUCache::find(int key)
{
    for (int i = 0; i < keys.size; i++)
    {
        if (keys.data[i] == key)
        {
            return i;
        }
    }
    return -1;
}

void LRUCache::moveToEnd(int index)
{
    int k = keys.data[index];
    int v = values.data[index];

    removeAt(&keys, index);
    removeAt(&values, index);

    pushBack(&keys, k);
    pushBack(&values, v);
}

int LRUCache::get(int key)
{
    int index = find(key);
    if (index == -1)
    {
        return -1;
    }

    int value = values.data[index];   // взяли значение, пока знаем где оно
    moveToEnd(index);                 // переставили
    return value;                     // вернули, что взяли
}

void LRUCache::set(int key, int value)
{
    int index = find(key);

    // ключ уже есть
    if (index != -1)
    {
        values.data[index] = value;   // новое значение
        moveToEnd(index);             // стал свежим
        return;
    }

    // ключа нет, а места нет
    if (keys.size == mesto)
    {
        removeAt(&keys, 0);           // самый старый - на месте 0
        removeAt(&values, 0);
    }

    // добавляем в конец
    pushBack(&keys, key);
    pushBack(&values, value);
}