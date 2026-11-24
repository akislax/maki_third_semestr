#include "Stack.h"
#include <stdexcept>

Stack::Stack()
{
    head = nullptr;
}

Stack::~Stack()
{
    while (!isEmptyStack())
    {
        pop();
    }
}

bool Stack::isEmptyStack() const
{
    return head == nullptr;
}

void Stack::push(const std::string& a)
{
    SNode* new_node = new SNode{a, head};
    head = new_node;
}

std::string Stack::pop()
{
    if (isEmptyStack())
    {
        throw std::runtime_error("Стек пуст");
    }
    SNode* tmp = head;
    std::string value = tmp -> a;
    head = head -> next;
    delete tmp;
    return value;
}

SNode* Stack::getHead() const
{
    return head;
}

