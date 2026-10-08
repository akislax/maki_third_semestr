#include "flist.h"

FNode* pushFront(FNode* head, const string& a)
{
    FNode* new_node = new FNode{a, head};
    return new_node;
}

FNode* popFront(FNode* head)
{
  if (head == nullptr) return nullptr;
  FNode* tmp = head;
  head = head -> next;
  delete tmp;
  return head;
}

void addOne(FNode* first, const string& a)
{
  FNode* new_node = new FNode{a, first -> next};
  first -> next = new_node;
}

void deleteNode(FNode* abc)
{
  if (abc -> next == nullptr) return;
  FNode* tmp = abc -> next;
  abc -> next = tmp -> next;
  delete tmp;

}

void freeList(FNode* head)
{
  while (head != nullptr)
  {
    FNode* tmp = head;
    head = head -> next;
    delete tmp;
  }
  
}

FNode* addTail(FNode* head, const string& value)
{
  FNode* new_node = new FNode{value, nullptr};
  
   if (head == nullptr)         
    {
      return new_node;
    }

  FNode* tmp = head;
  while (tmp -> next != nullptr)
  {

      tmp = tmp -> next;
  }
  tmp -> next = new_node;
  return head;
  
}

FNode* popTail(FNode* head)
{
    if (head == nullptr) return nullptr;

    if (head -> next == nullptr)
    {
        delete head;
        return nullptr;
    }

    FNode* tmp = head;
    while (tmp -> next -> next != nullptr)
    {
        tmp = tmp -> next;
    }

    delete tmp -> next;
    tmp -> next = nullptr;
    return head;
}

FNode* addBefore(FNode* head, FNode* target, const string& a)
{
    if (target == head) return pushFront(head, a);

    FNode* p = head;
    while (p -> next != target)
    {
        p = p -> next;
    }
    addOne(p, a);
    return head;
}

FNode* deleteBefore(FNode* head, FNode* target)
{
    if (head == nullptr || target == head) return head;

    if (head -> next == target) return popFront(head);

    FNode* p = head;
    while (p -> next -> next != target)
    {
        p = p -> next;
    }
    deleteNode(p);
    return head;
}

FNode* findValue(FNode* head, const string& a)
{
    FNode* current = head;
    while (current != nullptr)
    {
        if (current -> value == a) return current;
        current = current -> next;
    }
    return nullptr;
}

FNode* deleteValue(FNode* head, const string& a)
{
    if (head == nullptr) return head;

    if (head -> value == a) return popFront(head);

    FNode* p = head;
    while (p -> next != nullptr && p -> next -> value != a)
    {
        p = p -> next;
    }

    if (p -> next != nullptr) deleteNode(p);
    return head;
}

void printReverse(FNode* head)
{
    if (head == nullptr) return;
    printReverse(head -> next);
    cout << head -> value << " ";
}

void printList(FNode* head)
{
    for (FNode* p = head; p != nullptr; p = p -> next)
    {
        cout << p -> value << " -> ";
    }
    cout << "nullptr\n";
}