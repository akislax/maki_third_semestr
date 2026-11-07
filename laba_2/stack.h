#pragma once
#include <string>
using namespace std;

template <typename T>
struct SNode
{
    T a;
    SNode<T>* next;
};

template <typename T>
struct Stack
{
    SNode<T>* head = nullptr;
};

template <typename T>
bool isEmptyStack(Stack<T>* stack);

template <typename T>
void push(Stack<T>* stack, const T& a);

template <typename T>
void pop(Stack<T>* stack);

template <typename T>
T top(Stack<T>* stack);

template <typename T>
void freeStack(Stack<T>* stack);

template <typename T>
void print(Stack<T>* stack);

int priority(char op);

template <typename T>
void operation(Stack<T>* stack, char op);
int schet(const string& primer);


