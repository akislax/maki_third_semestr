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

void handleStack(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "stack")) return;

    Stack stack;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            push(&stack, element.get<string>());
        }
    }

    string value;
    switch (command)
    {
        case Command::SPUSH:
            ss >> value;
            push(&stack, value);
            cout << "-> " << value << "\n";
            break;
        case Command::SPOP:
            if (isEmptyStack(&stack)) cout << "-> стек пуст\n";
            else
            {
                cout << "-> " << top(&stack) << "\n";
                pop(&stack);
            }
            break;
        case Command::SPRINT:
            print(&stack);
            break;
        default:
            break;
    }

    json data = json::array();
    for (SNode* current = stack.head; current != nullptr; current = current -> next)
    {
        data.insert(data.begin(), current -> a);
    }
    db[name]["type"] = "stack";
    db[name]["data"] = data;

    freeStack(&stack);
}
