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

void handleFlist(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "flist")) return;

    FNode* head = nullptr;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            head = addTail(head, element.get<string>());
        }
    }

    string target, value;
    FNode* node = nullptr;
    switch (command)
    {
        case Command::FPUSHH:
            ss >> value;
            head = pushFront(head, value);
            break;
        case Command::FPUSHT:
            ss >> value;
            head = addTail(head, value);
            break;
        case Command::FPUSHA:
            ss >> target >> value;
            node = findValue(head, target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else addOne(node, value);
            break;
        case Command::FPUSHB:
            ss >> target >> value;
            node = findValue(head, target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else head = addBefore(head, node, value);
            break;
        case Command::FDELH:
            head = popFront(head);
            break;
        case Command::FDELT:
            head = popTail(head);
            break;
        case Command::FDELA:
            ss >> target;
            node = findValue(head, target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else deleteNode(node);
            break;
        case Command::FDELB:
            ss >> target;
            node = findValue(head, target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else head = deleteBefore(head, node);
            break;
        case Command::FDEL:
            ss >> value;
            head = deleteValue(head, value);
            break;
        case Command::FFIND:
            ss >> value;
            if (findValue(head, value) != nullptr) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::FPRINT:
            printList(head);
            break;
        case Command::FPRINTR:
            printReverse(head);
            cout << "\n";
            break;
        default:
            break;
    }

    json data = json::array();
    for (FNode* current = head; current != nullptr; current = current -> next)
    {
        data.push_back(current -> value);
    }
    db[name]["type"] = "flist";
    db[name]["data"] = data;

    freeList(head);
}