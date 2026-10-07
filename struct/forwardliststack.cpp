#include <iostream>

using namespace std;

struct Node
{
  int a;
  Node* next;
};

struct Stack
{
  Node* head = nullptr;
};

bool isEmptyStack(Stack* stack)
{
  return stack -> head == nullptr;
}

void push(Stack* stack, int a)
{
  Node* new_node = new Node{a, stack -> head};
  stack -> head = new_node;
}

void pop(Stack* stack)
{
  if (isEmptyStack(stack)) return;
  Node* tmp = stack -> head;
  stack -> head = stack -> head -> next;
  delete tmp;
}

int top(Stack* stack)
{
  if (isEmptyStack(stack)) return -1;
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
  Node* current = stack -> head;
  while (current != nullptr)
  {
    cout << current -> a << " (" << current << ") -> ";
    current = current -> next;
  }
  cout << "nullptr\n";
}

int main()
{
  Stack stack;
  cout << "Пустой:     "; print(&stack);

  push(&stack, 1);
  push(&stack, 2);
  push(&stack, 3);
  cout << "push 1 2 3: "; print(&stack);

  cout << "top: " << top(&stack) << "\n";

  pop(&stack);
  cout << "pop:        "; print(&stack);

  freeStack(&stack);
  cout << "free:       "; print(&stack);

  return 0;
}