#include "dlistClass.h"

Dlist::Dlist()
{
    head = nullptr;
    tail = nullptr;
}

Dlist::~Dlist()
{
    while (!isEmptyList())
    {
        popBack();
    }
}

bool Dlist::isEmptyList() const
{
    return head == nullptr && tail == nullptr;
}

void Dlist::pushBack(const std::string& name)
{
    DNode* new_node = new DNode{name, nullptr, nullptr};

    if (isEmptyList())
    {
        head = new_node;
        tail = new_node;

    }
    else
    {
        tail -> next = new_node;
        new_node -> prev = tail;
        tail = new_node;
    }
}

void Dlist::popBack()
{
    if (isEmptyList()) return;
    DNode* tmp = tail;
    tail = tail -> prev;
    if (tail == nullptr)      // узел был единственным
    {
        head = nullptr;       // список опустел
    }
    else
    {
        tail->next = nullptr; // за новым хвостом никого
    }
    delete tmp;
}



void Dlist::pushFront(const std::string& name)
{
    DNode* new_node = new DNode{name, nullptr, nullptr};

    if (isEmptyList())
    {
        head = new_node;
        tail = new_node;
    }
    else
    {
        head->prev = new_node;   // старая голова: передо мной новый
        new_node->next = head;   // новый: за мной старая голова
        head = new_node;         // новый стал головой
    }
}

void Dlist::popFront()
{
    if (isEmptyList()) return;

    DNode* tmp = head;
    head = tmp->next;

    if (head == nullptr)         // узел был единственным
    {
        tail = nullptr;          // список опустел
    }
    else
    {
        head->prev = nullptr;    // перед новой головой никого
    }

    delete tmp;
}

void Dlist::pushBeetwen (DNode* list, const std::string& name)
{
    if (list == nullptr) return;
    DNode* new_node = new DNode {name, nullptr, nullptr};

    new_node -> next = list -> next;

    new_node -> prev = list;

    if (list -> next != nullptr)
    {
    list -> next -> prev = new_node;
    }
    else 
    {
        tail = new_node;
    }
    list -> next = new_node;
}

void Dlist::pushBefore(DNode* list, const std::string& name)
{
    if (list == nullptr) return;
    
    if (list -> prev == nullptr)
    {
        pushFront(name);
    }
    else
    {
        pushBeetwen(list -> prev, name);
    }
}

void Dlist::popAfter(DNode* list)
{
    if (list == nullptr || list -> next == nullptr) return;
    
    DNode* tmp = list -> next;

    if (tmp -> next == nullptr)
    {
        list -> next = nullptr;
        tail = list;
        delete tmp;
        return;
    }
    
    list -> next = tmp -> next;

    tmp -> next -> prev = list;

    delete tmp;

}

void Dlist::popBefore(DNode* list)
{
    if (list == nullptr || list -> prev == nullptr) return;

    DNode* tmp = list -> prev;

    if (tmp -> prev == nullptr)
    {
        list -> prev = nullptr;
        head = list;
        delete tmp;
        return;
    }

    tmp -> prev -> next = list;
    list -> prev = tmp -> prev;

    delete tmp;
}

void Dlist::printForward() const
{
    DNode* current = head;
    while (current != nullptr)
    {
        std::cout << current->person << " <-> ";
        current = current->next;     // шаг к концу
    }
    std::cout << "nullptr" << '\n';
}

void Dlist::printBackward() const
{
    DNode* current = tail;
    while (current != nullptr)
    {
        std::cout << current -> person << " <-> ";
        current = current -> prev;
    }
    std::cout << "nullptr" << '\n';
}

DNode* Dlist::findName(const std::string& name) const
{
    DNode* current = head;

    while (current != nullptr)
    {
        if (current -> person == name)
        {
            return current;
        }
        current = current -> next;
    }
    return nullptr;
}

void Dlist::popName(const std::string& name)
{
    DNode* node = findName(name);
    if (node == nullptr) 
    {
        std::cout << "нет такого элемента\n";
        return;
    }
    if (node == head)
    {
        popFront();
    }
    else
    {
        popAfter(node -> prev);
    }
    
}

DNode* Dlist::getHead() const
{
    return head;
}

void handleDlist(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "dlist")) return;

    Dlist dl;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            dl.pushBack(element.get<string>());
        }
    }

    string target;
    string value;
    DNode* node = nullptr;
    switch (command)
    {
        case Command::LPUSHH:
            ss >> value;
            dl.pushFront(value);
            break;
        case Command::LPUSHT:
            ss >> value;
            dl.pushBack(value);
            break;
        case Command::LPUSHA:
            ss >> target >> value;
            node = dl.findName(target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else dl.pushBeetwen(node, value);
            break;
        case Command::LPUSHB:
            ss >> target >> value;
            node = dl.findName(target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else dl.pushBefore(node, value);
            break;
        case Command::LDELH:
            dl.popFront();
            break;
        case Command::LDELT:
            dl.popBack();
            break;
        case Command::LDELA:
            ss >> target;
            node = dl.findName(target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else dl.popAfter(node);
            break;
        case Command::LDELB:
            ss >> target;
            node = dl.findName(target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else dl.popBefore(node);
            break;
        case Command::LDEL:
            ss >> value;
            dl.popName(value);
            break;
        case Command::LFIND:
            ss >> value;
            if (dl.findName(value) != nullptr) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::LPRINT:
            dl.printForward();
            break;
        case Command::LPRINTR:
            dl.printBackward();
            break;
        default:
            break;
    }

    json data = json::array();
    for (DNode* current = dl.getHead(); current != nullptr; current = current -> next)
    {
        data.push_back(current -> person);
    }
    db[name]["type"] = "dlist";
    db[name]["data"] = data;

}