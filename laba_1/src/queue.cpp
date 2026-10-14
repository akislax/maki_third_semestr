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

void handleQueue(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "queue")) return;

    Q q;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            push(&q, element.get<string>());
        }
    }

    string value;
    switch (command)
    {
        case Command::QPUSH:
            ss >> value;
            push(&q, value);
            cout << "-> " << value << "\n";
            break;
        case Command::QPOP:
            if (isEmptyQ(&q)) cout << "-> очередь пуста\n";
            else
            {
                cout << "-> " << top(&q) << "\n";
                pop(&q);
            }
            break;
        case Command::QPRINT:
            printQ(&q);
            break;
        default:
            break;
    }

    json data = json::array();
    for (QNode* current = q.head; current != nullptr; current = current -> next)
    {
        data.push_back(current -> a);
    }
    db[name]["type"] = "queue";
    db[name]["data"] = data;

    freeQ(&q);
}
