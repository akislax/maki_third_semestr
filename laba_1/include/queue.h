#pragma once
#include <iostream>
#include <string>
using namespace std;

struct QNode 
{
  string a;
  QNode* next;
};

struct Q 
{
  QNode* head = nullptr;
  QNode* tail = nullptr;
};

bool isEmptyQ(Q* q);
void push(Q* q, const string& a);
void pop(Q* q);
string top(Q* q);
void freeQ(Q* q);
void printQ(Q* q);