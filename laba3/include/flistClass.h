#pragma once
#include "serialize.h"
#include <vector>
#include "commandsClass.h"
#include <iostream>
#include <string>
using namespace std;

struct FNode
{
    string value;
    FNode *next = nullptr;
};

class Flist
{
private:
    FNode *head = nullptr;
    void printReverseFrom(FNode *node) const;   // рекурсивный помощник для printReverse

public:
    void saveBinary(const string &fileName) const;
    void loadBinary(const string &fileName);
    Flist();
    ~Flist();
    Flist(const Flist&) = delete;
    Flist& operator=(const Flist&) = delete;

    bool isEmptyList() const;
    void pushFront(const string &a);
    void popFront();
    void addTail(const string &value);
    void popTail();
    void addOne(FNode *first, const string &a);      // вставка после узла
    void deleteNode(FNode *abc);                      // удаление после узла
    void addBefore(FNode *target, const string &a);
    void deleteBefore(FNode *target);
    FNode *findValue(const string &a) const;
    void deleteValue(const string &a);
    void printList() const;
    void printReverse() const;
    FNode *getHead() const;
};

void handleFlist(Command command, json& db, const string& name, stringstream& ss);
