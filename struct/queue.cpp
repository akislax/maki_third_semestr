#include <iostream>
#include <string>


struct Node
{
    std::string person;
    Node* next;
};

struct Queue
{
    Node* head;
    Node* tail;  
};

void initQueue(Queue* queue)
{
    queue -> head = nullptr;
    queue -> tail = nullptr;
}
bool isEmptyQueue(Queue* queue)
{
    if (queue -> head == nullptr) return true;
    else return false;
}
void enqueue(Queue* queue, const std::string& name)
{
    Node* new_node = new Node{name, nullptr};   // новый встаёт последним, за ним никого

    if (isEmptyQueue(queue))
    {
        queue->head = new_node;
        queue->tail = new_node;
    }
    else
    {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
}
void dequeue(Queue* queue)
{
    if (isEmptyQueue(queue)) return;     // удалять нечего

    Node* tmp = queue->head;             // запомнили первого
    queue->head = tmp->next;             // первым стал следующий
    delete tmp;                          // удалили

    if (queue->head == nullptr)          // очередь опустела?
    {
        queue->tail = nullptr;           // тогда и хвост обнуляем
    }
}

void freeQueue(Queue* queue)
{
    while (!isEmptyQueue(queue))
    {
        dequeue(queue);
    }
}

void print(Queue* queue)
{
    std::cout << "  head = " << queue->head << ", tail = " << queue->tail << std::endl;
    std::cout << "  ";
    Node* current = queue->head;
    while (current != nullptr)
    {
        std::cout << current->person << " (" << current << ") -> ";
        current = current->next;
    }
    std::cout << "nullptr" << std::endl;
}

int main()
{
    Queue queue;
    initQueue(&queue);
    std::cout << "Пустая:" << std::endl;
    print(&queue);

    enqueue(&queue, "Nusha");
    enqueue(&queue, "Krosh");
    enqueue(&queue, "Ejik");
    std::cout << "После трёх enqueue:" << std::endl;
    print(&queue);

    dequeue(&queue);
    std::cout << "После dequeue:" << std::endl;
    print(&queue);

    freeQueue(&queue);
    std::cout << "После freeQueue:" << std::endl;
    print(&queue);

    return 0;
}