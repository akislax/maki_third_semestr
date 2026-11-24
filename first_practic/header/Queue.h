#pragma once
#include "string"

struct QNode
{
    std::string a;
    QNode *next = nullptr;
};

class Queue
{
private:
    QNode *head = nullptr;
    QNode *tail = nullptr;

public:
    Queue();
    ~Queue();
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    bool isEmptyQ() const;
    void push(const std::string &a);
    std::string pop();
    QNode* getHead() const;
};

