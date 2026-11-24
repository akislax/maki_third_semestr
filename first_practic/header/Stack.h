#pragma once
#include <string>
struct SNode
{
    std::string a;
    SNode *next = nullptr;
};

class Stack
{
private:
    SNode *head = nullptr;

public:
    Stack();
    ~Stack();

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    bool isEmptyStack() const;
    void push(const std::string &a);
    std::string pop();
    SNode* getHead() const;
};
