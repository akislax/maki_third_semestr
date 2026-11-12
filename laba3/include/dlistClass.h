#pragma once
#include "serialize.h"
#include <vector>
#include "commandsClass.h"
#include <iostream>
#include <string>
using namespace std;

struct DNode
{
    std::string person;
    DNode *next = nullptr;
    DNode *prev = nullptr;
};

class Dlist
{
    private:
    DNode *head = nullptr;
    DNode *tail = nullptr;

public:
    void saveBinary(const string &fileName) const;
    void loadBinary(const string &fileName);
    Dlist();
    ~Dlist();
    Dlist(const Dlist&) = delete;
    Dlist& operator=(const Dlist&) = delete;
    bool isEmptyList() const;
    void pushBack(const std::string &name);
    void popBack();
    void pushFront(const std::string &name);
    void popFront();
    void pushBeetwen(DNode *list, const std::string &name);
    void pushBefore(DNode *list, const std::string &name);
    void popAfter(DNode *list);
    void popBefore(DNode *list);
    void printForward() const;
    void printBackward() const;
    DNode *findName(const std::string &name) const;
    void popName(const std::string &name);
    DNode *getHead() const;
};

void handleDlist(Command command, json &db, const string &name, stringstream &ss);