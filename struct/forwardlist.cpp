#include <iostream>
#include <string>

struct Node
{
    std::string person;
    Node* next;
};

void freeList(Node* head)
{
    while (head != nullptr)
    {
        Node* tmp = head;
        head = head -> next;
        delete tmp;
    }
}

Node* pushFront (Node* head, const std::string& name)
{
    Node* new_node = new Node {name, head};
    return new_node;
}

void addOne(Node* first, const std::string& name)
{
    Node* new_node = new Node{name, first -> next};
    first -> next = new_node;
}

Node* popFront(Node* head)
{
    if (head == nullptr) return nullptr;  // список пуст, удалять нечего
    Node* tmp = head;
    head = head->next;
    delete tmp;
    return head;                          // возвращаем новую голову
}

void DeleteNode(Node* first)
{
    if (first -> next == nullptr) return;

    Node* tmp = first -> next;
    first -> next = tmp -> next;
    delete tmp;
}
void print(Node* head)
{
    while (head != nullptr)
    {
        std::cout << head->person << " (" << head << ") -> ";
        head = head->next;
    }
    std::cout << "nullptr" << std::endl;
}

int main()
{
    Node* head = new Node{"Nusha", nullptr};
    head->next = new Node{"Krosh", nullptr};
    head->next->next = new Node{"Ejik", nullptr};
    std::cout << "Начало:            "; print(head);

    head = pushFront(head, "Losyash");
    std::cout << "pushFront Losyash: "; print(head);

    head = popFront(head);
    std::cout << "popFront:          "; print(head);

    addOne(head, "Sovunya");            // после Nusha
    std::cout << "addOne Sovunya:    "; print(head);

    DeleteNode(head);                   // удалит того, кто после Nusha (Sovunya)
    std::cout << "DeleteNode:        "; print(head);

    freeList(head);
    head = nullptr;
    std::cout << "После freeList:    "; print(head);
    return 0;
}