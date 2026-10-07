#include <iostream>
using namespace std;

struct Node
{
  int number;
  Node* next;
};

Node* pushFront(Node* head, int a)
{
    Node* new_node = new Node{a, head};
    return new_node;
}

Node* popFront(Node* head)
{
  if (head == nullptr) return nullptr;
  Node* tmp = head;
  head = head -> next;
  delete tmp;
  return head;
}

void addOne(Node* first, int a)
{
  Node* new_node = new Node{a, first -> next};
  first -> next = new_node;
}

void deleteNode(Node* abc)
{
  if (abc -> next == nullptr) return;
  Node* tmp = abc -> next;
  abc -> next = tmp -> next;
  delete tmp;

}

void freeList(Node* head)
{
  while (head != nullptr)
  {
    Node* tmp = head;
    head = head -> next;
    delete tmp;
  }
  
}

Node* addTail(Node* head, int number)
{
  Node* new_node = new Node{number, nullptr};
  
   if (head == nullptr)         
    {
      return new_node;
    }

  Node* tmp = head;
  while (tmp -> next != nullptr)
  {

      tmp = tmp -> next;
  }
  tmp -> next = new_node;
  return head;
  
}

Node* popTail(Node* head)
{
    if (head == nullptr) return nullptr;

    if (head -> next == nullptr)
    {
        delete head;
        return nullptr;
    }

    Node* tmp = head;
    while (tmp -> next -> next != nullptr)
    {
        tmp = tmp -> next;
    }

    delete tmp -> next;
    tmp -> next = nullptr;
    return head;
}

Node* addBefore(Node* head, Node* target, int a)
{
    if (target == head) return pushFront(head, a);

    Node* p = head;
    while (p -> next != target)
    {
        p = p -> next;
    }
    addOne(p, a);
    return head;
}

Node* deleteBefore(Node* head, Node* target)
{
    if (head == nullptr || target == head) return head;

    if (head -> next == target) return popFront(head);

    Node* p = head;
    while (p -> next -> next != target)
    {
        p = p -> next;
    }
    deleteNode(p);
    return head;
}

Node* findValue(Node* head, int a)
{
    Node* current = head;
    while (current != nullptr)
    {
        if (current -> number == a) return current;
        current = current -> next;
    }
    return nullptr;
}

Node* deleteValue(Node* head, int a)
{
    if (head == nullptr) return head;

    if (head -> number == a) return popFront(head);

    Node* p = head;
    while (p -> next != nullptr && p -> next -> number != a)
    {
        p = p -> next;
    }

    if (p -> next != nullptr) deleteNode(p);
    return head;
}

void printReverse(Node* head)
{
    if (head == nullptr) return;
    printReverse(head -> next);
    cout << head -> number << " ";
}

void printList(Node* head)
{
    for (Node* p = head; p != nullptr; p = p -> next)
    {
        cout << p -> number << " -> ";
    }
    cout << "nullptr\n";
}

int main()
{
  Node* head = new Node{1, nullptr};
  head -> next = new Node{2, nullptr};
  head -> next -> next = new Node{3, nullptr};
  head -> next -> next -> next = new Node{4, nullptr};
  cout << "Создали:       ";
  printList(head);

  head = pushFront(head, 0);
  cout << "pushFront(0):  ";
  printList(head);

  head = popFront(head);
  cout << "popFront:      ";
  printList(head);

  addOne(head, 67);
  cout << "addOne(67):    ";
  printList(head);

  deleteNode(head);
  cout << "deleteNode:    ";
  printList(head);

  freeList(head);
  cout << "freeList:      ";
  printList(head);

  return 0;
}