#include "queue.h"

bool isEmptyQ(Q* q)
{
  if (q -> head == nullptr) return true;
  else return false;
}

void push(Q* q, const string& a)
{
  QNode* new_node = new QNode {a, nullptr};

  if (isEmptyQ(q))
  {
    q -> head = new_node;
    q -> tail = new_node;
  }
  else
  {
    q -> tail -> next = new_node;
    q -> tail = new_node;
  }
}

void pop(Q* q)
{
  if (isEmptyQ(q)) return;

  QNode* tmp = q -> head;
  q -> head = q -> head -> next;
  delete tmp;

  if (isEmptyQ(q))
  {
    q -> tail = nullptr;
  }
}

string top(Q* q)
{
  if (isEmptyQ(q)) return "";
  else
  {
    return q -> head -> a;
  }
}

void freeQ(Q* q)
{
  while(!isEmptyQ(q))
    {
      pop(q);
    }
}

void printQ(Q* q)
{
  QNode* current = q -> head;
  while (current != nullptr)
  {
  cout << current -> a << " -> ";
  current = current -> next;
  }
  cout << "nullptr\n";
  
}