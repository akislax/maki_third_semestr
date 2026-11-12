#pragma once
#include "serialize.h"
#include <vector>
#include "commandsClass.h"
#include <iostream>
#include <string>
using namespace std;

struct QNode
{
    string a;
    QNode *next = nullptr;
};

class Queue
{
private:
    QNode *head = nullptr;
    QNode *tail = nullptr;

public:
    void saveBinary(const string &fileName) const;
    void loadBinary(const string &fileName);
    Queue();
    ~Queue();
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    bool isEmptyQ() const;
    void push(const string &a);
    void pop();
    string top() const;
    void printQ() const;
    QNode *getHead() const;
};

void handleQueue(Command command, json& db, const string& name, stringstream& ss);
