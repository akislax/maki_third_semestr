#pragma once
#include "serialize.h"
#include <vector>
#include "commandsClass.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class Array
{
private:
    string *data = nullptr;
    int size = 0;
    int capacity = 0;
    void resizeArray();
//вообще зачем запрет на копирование и присваивание? кгода мы скопировали то просто два объекта смотрят на одну и ту же память. у структур мы сами удаляем и не вызываем повторно, иначе была бы ошибка. а у классов деструктор вызывается автоматически и получилось бы двойное удаление что было бы ошибкой
public:
    void saveBinary(const string &fileName) const;
    void loadBinary(const string &fileName);
    Array();
    ~Array();
    Array(const Array&) = delete;             // запрет копирования: Array b = a;
    Array& operator=(const Array&) = delete;  // запрет присваивания: b = a;
    void pushBack(const string &text);
    void insertAt(const string &text, int index);
    void removeAt(int index);
    void setAt(const string &text, int index);
    string getAt(int index) const;
    void printArray() const;
    int lenght() const;
};

void handleArray(Command command, json& db, const string& name, stringstream& ss);
