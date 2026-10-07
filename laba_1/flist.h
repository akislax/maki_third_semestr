#pragma once
#include <iostream>
#include <string>
using namespace std;

struct FNode
{
  string value;
  FNode* next;
};

FNode* pushFront(FNode* head, const string& a);
FNode* popFront(FNode* head);
void addOne(FNode* first, const string& a);
void deleteNode(FNode* abc);
void freeList(FNode* head);
FNode* addTail(FNode* head, const string& value);
FNode* popTail(FNode* head);
FNode* addBefore(FNode* head, FNode* target, const string& a);
FNode* deleteBefore(FNode* head, FNode* target);
FNode* findValue(FNode* head, const string& a);
FNode* deleteValue(FNode* head, const string& a);
void printReverse(FNode* head);
void printList(FNode* head);