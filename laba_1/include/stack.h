#pragma once
#include <iostream>
#include <string>
using namespace std;

struct SNode
{
  string a;
  SNode* next;
};

struct Stack
{
  SNode* head = nullptr;
};

bool isEmptyStack(Stack* stack);
void push(Stack* stack, const string& a);
void pop(Stack* stack);
string top(Stack* stack);
void freeStack(Stack* stack);
void print(Stack* stack);