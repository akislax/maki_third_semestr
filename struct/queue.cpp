#include <iostream>

using namespace std;

struct Node 
{
  int a;
  Node* next;
};

struct Q 
{
  Node* head = nullptr;
  Node* tail = nullptr;
};

bool isEmptyQ(Q* q)
{
  if (q -> head == nullptr) return true;
  else return false;
}

void push(Q* q, int a)
{
  Node* new_node = new Node {a, nullptr};

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

  Node* tmp = q -> head;
  q -> head = q -> head -> next;
  delete tmp;

  if (isEmptyQ(q))
  {
    q -> tail = nullptr;
  }
}

int top(Q* q)
{
  if (isEmptyQ(q)) return -1;
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
  Node* current = q -> head;
  while (current != nullptr)
  {
  cout << current -> a << " (" << current << ") ->";
  current = current -> next;
  }
  cout << "\n";
  
}

int main()
{
  Q q;
  push(&q, 1);
  printQ(&q);
  push(&q, 2);
  printQ(&q);
  push(&q, 3);
  printQ(&q);
  push(&q, 4);
  printQ(&q);
  pop(&q);
  printQ(&q);
  cout << top(&q) << "\n";
  freeQ(&q);
  printQ(&q);
  return 0;
}