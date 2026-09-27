#include <iostream>
#include <string>

struct DNode
{
    std::string person;
    DNode* next;
    DNode* prev;
};

struct Dlist
{
    DNode* head;
    DNode* tail;
};

void init2hand(Dlist* dlist)
{
    dlist -> head = nullptr;
    dlist -> tail = nullptr;
}

bool isEmptyList(Dlist* dlist)
{
    if (dlist -> head == nullptr && dlist -> tail == nullptr) return true;
    else return false;
}

void pushBack(Dlist* dlist, const std::string& name)
{
    DNode* new_node = new DNode{name, nullptr, nullptr};

    if (isEmptyList(dlist))
    {
        dlist -> head = new_node;
        dlist -> tail = new_node;

    }
    else
    {
        dlist -> tail -> next = new_node;
        new_node -> prev = dlist -> tail;
        dlist -> tail = new_node;
    }
}

void popBack(Dlist* dlist)
{
    if (isEmptyList(dlist)) return;
    DNode* tmp = dlist -> tail;
    dlist -> tail = dlist -> tail -> prev;
        if (dlist->tail == nullptr)      // узел был единственным
    {
        dlist->head = nullptr;       // список опустел
    }
    else
    {
        dlist->tail->next = nullptr; // за новым хвостом никого
    }
    delete tmp;
}

void printForward(Dlist* dlist)
{
    DNode* current = dlist->head;
    while (current != nullptr)
    {
        std::cout << current->person << " <-> ";
        current = current->next;     // шаг к концу
    }
    std::cout << "nullptr" << std::endl;
}

void printBackward(Dlist* dlist)
{
    DNode* current = dlist -> tail;
    while (current != nullptr)
    {
        std::cout << current -> person << " <-> ";
        current = current -> prev;
    }
    std::cout << "nullptr" << std::endl;
}

void freeList(Dlist* dlist)
{
    while (!isEmptyList(dlist))
    {
        popBack(dlist);
    }
}

void pushFront(Dlist* dlist, const std::string& name)
{
    DNode* new_node = new DNode{name, nullptr, nullptr};

    if (isEmptyList(dlist))
    {
        dlist->head = new_node;
        dlist->tail = new_node;
    }
    else
    {
        dlist->head->prev = new_node;   // старая голова: передо мной новый
        new_node->next = dlist->head;   // новый: за мной старая голова
        dlist->head = new_node;         // новый стал головой
    }
}

void popFront(Dlist* dlist)
{
    if (isEmptyList(dlist)) return;

    DNode* tmp = dlist->head;
    dlist->head = tmp->next;

    if (dlist->head == nullptr)         // узел был единственным
    {
        dlist->tail = nullptr;          // список опустел
    }
    else
    {
        dlist->head->prev = nullptr;    // перед новой головой никого
    }

    delete tmp;
}

int main()
{
    Dlist list;
    init2hand(&list);

    // добавляем с обоих концов
    pushBack(&list, "Krosh");     //            Krosh
    pushBack(&list, "Ejik");      //            Krosh, Ejik
    pushFront(&list, "Nusha");    //     Nusha, Krosh, Ejik
    pushFront(&list, "Losyash");  // Losyash, Nusha, Krosh, Ejik

    std::cout << "Вперёд: "; printForward(&list);
    std::cout << "Назад:  "; printBackward(&list);

    popFront(&list);              // ушёл Losyash
    std::cout << "После popFront: "; printForward(&list);

    popBack(&list);               // ушёл Ejik
    std::cout << "После popBack:  "; printForward(&list);

    freeList(&list);
    std::cout << "После freeList: "; printForward(&list);
    return 0;
}