#include <iostream>
#include <string>

struct Node
{
    std::string name;
    Node* next;
};

struct Stack
{
    Node* head;
};

void InitStack(Stack* stack) //кладет в переменную stack адрес
{
    stack -> head = nullptr;
}
bool isEmptyStack(Stack* stack)
{
    if (stack -> head == nullptr) return true;
    else return false;
}

void push(Stack* stack, const std::string& name)
{
    Node* new_node = new Node{name, stack -> head};
    stack -> head = new_node;
}

void pop(Stack* stack)
{
    if (stack -> head == nullptr) return;
    Node* tmp = stack -> head;
    stack -> head = stack -> head -> next;
    delete tmp;

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
    Node* current = stack->head;
    while (current != nullptr)
    {
        std::cout << current->name << " (" << current << ") -> ";
        current = current->next;
    }
    std::cout << "nullptr" << std::endl;
}

int main()
{
    Stack stack;                  // коробка в main
    InitStack(&stack);            // head = nullptr, стек пустой
    std::cout << "Пустой:       "; print(&stack);

    push(&stack, "Sovunya");      // new: Sovunya в куче
    push(&stack, "Losyash");      // new: Losyash в куче
    push(&stack, "Krosh");        // new: Krosh в куче
    std::cout << "После push:   "; print(&stack);

    pop(&stack);                  // delete: снимаем верхний
    std::cout << "После pop:    "; print(&stack);

    freeStack(&stack);            // delete всех оставшихся
    std::cout << "После free:   "; print(&stack);

    return 0;
}