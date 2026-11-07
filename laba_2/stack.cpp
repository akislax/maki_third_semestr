#include "stack.h"
#include <iostream>
#include <string>
using namespace std;

template <typename T>
bool isEmptyStack(Stack<T>* stack)
{
    return stack->head == nullptr;
}

template <typename T>
void push(Stack<T>* stack, const T& a)
{
    SNode<T>* new_node = new SNode<T>{a, stack->head};
    stack->head = new_node;
}

template <typename T>
void pop(Stack<T>* stack)
{
    if (isEmptyStack(stack)) return;
    SNode<T>* tmp = stack->head;
    stack->head = stack->head->next;
    delete tmp;
}

template <typename T>
T top(Stack<T>* stack)
{
    if (isEmptyStack(stack)) return T();
    return stack->head->a;
}

template <typename T>
void freeStack(Stack<T>* stack)
{
    while (!isEmptyStack(stack))
    {
        pop(stack);
    }
}

template <typename T>
void print(Stack<T>* stack)
{
    SNode<T>* current = stack->head;
    while (current != nullptr)
    {
        cout << current->a << " -> ";
        current = current->next;
    }
    cout << "nullptr\n";
}

int priority(char op)
{
    if (op == '!') return 3;
    if (op == '&') return 2;
    if (op == '|' || op == '^') return 1;
    return 0;
}

void operation(Stack<int>* stack, char op)
{
    if (op == '!')
    {
        int a = top(stack);
        pop(stack);
        push(stack, a == 1 ? 0 : 1);
        return;
    }

    int b = top(stack); pop(stack);
    int a = top(stack); pop(stack);

    int result = 0;
    if (op == '&') result = a & b;
    if (op == '|') result = a | b;
    if (op == '^') result = a ^ b;

    push(stack, result);
}

int schet(const string& primer)
{
    Stack<int> nums;
    Stack<char> ops;

    for (char c : primer)
    {
        if (c == ' ') continue;

        if (c == '0' || c == '1')
        {
            push(&nums, c - '0');
        }
        else if (c == '(' || c == '!')
        {
            push(&ops, c);
        }
        else if (c == ')')
        {
            while (top(&ops) != '(')
            {
                operation(&nums, top(&ops));
                pop(&ops);
            }
            pop(&ops);
        }
        else if (c == '&' || c == '|' || c == '^')
        {
            while (!isEmptyStack(&ops) && top(&ops) != '('
                   && priority(top(&ops)) >= priority(c))
            {
                operation(&nums, top(&ops));
                pop(&ops);
            }
            push(&ops, c);
        }
    }  

    while (!isEmptyStack(&ops))
    {
        operation(&nums, top(&ops));
        pop(&ops);
    }

    int result = top(&nums);
    freeStack(&nums);
    return result;
}
