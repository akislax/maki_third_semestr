#include "Queue.h"
#include <stdexcept>

Queue::Queue()
{
    head = nullptr;
    tail = nullptr;
}

Queue::~Queue()
{
    while (!isEmptyQ())
    {
        pop();
    }
}

bool Queue::isEmptyQ() const
{
    return head == nullptr;
}

void Queue::push(const std::string &a)
{
    QNode *new_node = new QNode{a, nullptr};

    if (isEmptyQ())
    {
        head = new_node;
        tail = new_node;
    }
    else
    {
        tail->next = new_node;
        tail = new_node;
    }
}

std::string Queue::pop()
{
    if (isEmptyQ())
    {
        throw std::runtime_error("Очередь пуста");
    }

    QNode *tmp = head;
    std::string value = tmp->a;

    head = head->next;
    delete tmp;

    if (head == nullptr)
    {
        tail = nullptr;
    }

    return value;
}

QNode* Queue::getHead() const
{
    return head;
}