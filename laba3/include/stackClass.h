#pragma once
#include "serialize.h"
#include <vector>
#include "commandsClass.h"
#include <iostream>
#include <string>
using namespace std;

struct SNode
{
    string a;
    SNode *next = nullptr;
};

class Stack
{
private:
    SNode *head = nullptr;

public:
    void saveBinary(const string &fileName) const;
    void loadBinary(const string &fileName);
    Stack();
    ~Stack();
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    bool isEmptyStack() const;
    void push(const string &a);
    void pop();
    string top() const;
    void print() const;
    SNode *getHead() const;
};

void handleStack(Command command, json& db, const string& name, stringstream& ss);
