#pragma once
#include "array_template.h"

class LRUCache
{
    private:
    int mesto;
    Array<int> keys;
    Array<int> values;
    int find(int key);
    void moveToEnd(int index);

    public:
    LRUCache (int cap);
    ~LRUCache();
    int get(int key);
    void set(int key, int value);
};