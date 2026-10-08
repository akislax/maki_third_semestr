#include "stack.h"

bool isEmptyStack(Stack* stack)
{
  return stack -> head == nullptr;
}

void push(Stack* stack, const string& a)
{
  SNode* new_node = new SNode{a, stack -> head};
  stack -> head = new_node;
}

void pop(Stack* stack)
{
  if (isEmptyStack(stack)) return;
  SNode* tmp = stack -> head;
  stack -> head = stack -> head -> next;
  delete tmp;
}

string top(Stack* stack)
{
  if (isEmptyStack(stack)) return "";
  return stack -> head -> a;
}

void freeStack(Stack* stack)
{
  while (!isEmptyStack(stack))
  {
    pop(stack);
  }
}

void print(Stack* stack)
{
  SNode* current = stack -> head;
  while (current != nullptr)
  {
    cout << current -> a << " -> ";
    current = current -> next;
  }
  cout << "nullptr\n";
}