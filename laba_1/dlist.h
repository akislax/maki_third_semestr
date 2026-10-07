#pragma once
#include <iostream>
#include <string>

struct DNode
{
    std::string person;
    DNode* next;
    DNode* prev;
};

struct Dlist
{
    DNode* head = nullptr;
    DNode* tail = nullptr;
};

bool isEmptyList(Dlist* dlist);
void pushBack(Dlist* dlist, const std::string& name);
void popBack(Dlist* dlist);
void freeList(Dlist* dlist);
void pushFront(Dlist* dlist, const std::string& name);
void popFront(Dlist* dlist);
void pushBeetwen (DNode* list, Dlist* dlist, const std::string& name);
void pushBefore(DNode* list, Dlist* dlist, const std::string& name);
void popAfter(DNode* list, Dlist* dlist);
void popBefore(DNode* list, Dlist* dlist);
void printForward(Dlist* dlist);
void printBackward(Dlist* dlist);
DNode* findName(Dlist* dlist, const std::string& name);
void popName(Dlist* dlist, const std::string& name);