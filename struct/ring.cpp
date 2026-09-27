#include <iostream>
#include <string>

struct Node
{
    std::string person;
    Node* next;
};

// добавить узел в конец кольца (перед головой)
Node* pushBack(Node* head, const std::string& name)
{
    Node* new_node = new Node{name, nullptr};

    if (head == nullptr)             // кольцо пустое
    {
        new_node->next = new_node;   // единственный узел смотрит сам на себя
        return new_node;
    }

    Node* last = head;               // ищем последнего:
    while (last->next != head)       // того, кто смотрит на голову
    {
        last = last->next;
    }

    last->next = new_node;           // старый последний → новый
    new_node->next = head;           // новый → голова, кольцо замкнуто
    return head;
}

// печать кольца
void printRing(Node* head)
{
    if (head == nullptr)
    {
        std::cout << "пусто" << std::endl;
        return;
    }

    Node* current = head;
    while (true)
    {
        std::cout << current->person << " (" << current << ") -> ";
        current = current->next;
        if (current == head) break;  // вернулись к первому — выходим
    }
    std::cout << "(снова " << head->person << ")" << std::endl;
}

// удалить всё кольцо
void freeRing(Node* head)
{
    if (head == nullptr) return;

    Node* last = head;               // находим последнего
    while (last->next != head)
    {
        last = last->next;
    }
    last->next = nullptr;            // разрываем кольцо

    while (head != nullptr)          // теперь это обычный список
    {
        Node* tmp = head;
        head = head->next;
        delete tmp;
    }
}

int main()
{
    Node* head = nullptr;

    head = pushBack(head, "Nusha");
    head = pushBack(head, "Krosh");
    head = pushBack(head, "Ejik");
    head = pushBack(head, "Losyash");

    std::cout << "Кольцо: ";
    printRing(head);

    std::cout << "6 шагов от " << head->person << ": ";
    Node* current = head;
    for (int i = 0; i < 6; i++)
    {
        current = current->next;
        std::cout << current->person << " ";
    }
    std::cout << std::endl;

    freeRing(head);
    head = nullptr;
    std::cout << "После freeRing: ";
    printRing(head);
    return 0;
}