#include "flistClass.h"

Flist::Flist()
{
    head = nullptr;
}

Flist::~Flist()
{
    while (!isEmptyList())
    {
        popFront();
    }
}

bool Flist::isEmptyList() const
{
    return head == nullptr;
}

void Flist::pushFront(const string& a)
{
    FNode* new_node = new FNode{a, head};
    head = new_node;
}

void Flist::popFront()
{
    if (isEmptyList()) return;
    FNode* tmp = head;
    head = head -> next;
    delete tmp;
}

void Flist::addOne(FNode* first, const string& a)
{
    if (first == nullptr) return;
    FNode* new_node = new FNode{a, first -> next};
    first -> next = new_node;
}

void Flist::deleteNode(FNode* abc)
{
    if (abc == nullptr || abc -> next == nullptr) return;
    FNode* tmp = abc -> next;
    abc -> next = tmp -> next;
    delete tmp;
}

void Flist::addTail(const string& value)
{
    FNode* new_node = new FNode{value, nullptr};

    if (isEmptyList())
    {
        head = new_node;
        return;
    }

    FNode* tmp = head;
    while (tmp -> next != nullptr)
    {
        tmp = tmp -> next;
    }
    tmp -> next = new_node;
}

void Flist::popTail()
{
    if (isEmptyList()) return;

    if (head -> next == nullptr)
    {
        delete head;
        head = nullptr;
        return;
    }

    FNode* tmp = head;
    while (tmp -> next -> next != nullptr)
    {
        tmp = tmp -> next;
    }

    delete tmp -> next;
    tmp -> next = nullptr;
}

void Flist::addBefore(FNode* target, const string& a)
{
    if (target == nullptr) return;

    if (target == head)
    {
        pushFront(a);
        return;
    }

    FNode* p = head;
    while (p -> next != target)
    {
        p = p -> next;
    }
    addOne(p, a);
}

void Flist::deleteBefore(FNode* target)
{
    if (isEmptyList() || target == nullptr || target == head) return;

    if (head -> next == target)
    {
        popFront();
        return;
    }

    FNode* p = head;
    while (p -> next -> next != target)
    {
        p = p -> next;
    }
    deleteNode(p);
}

FNode* Flist::findValue(const string& a) const
{
    FNode* current = head;
    while (current != nullptr)
    {
        if (current -> value == a) return current;
        current = current -> next;
    }
    return nullptr;
}

void Flist::deleteValue(const string& a)
{
    if (isEmptyList()) return;

    if (head -> value == a)
    {
        popFront();
        return;
    }

    FNode* p = head;
    while (p -> next != nullptr && p -> next -> value != a)
    {
        p = p -> next;
    }

    if (p -> next != nullptr) deleteNode(p);
}

void Flist::printReverseFrom(FNode* node) const
{
    if (node == nullptr) return;
    printReverseFrom(node -> next);
    cout << node -> value << " ";
}

void Flist::printReverse() const
{
    printReverseFrom(head);
    cout << "\n";
}

void Flist::printList() const
{
    for (FNode* p = head; p != nullptr; p = p -> next)
    {
        cout << p -> value << " -> ";
    }
    cout << "nullptr\n";
}

FNode* Flist::getHead() const
{
    return head;
}

void handleFlist(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "flist")) return;

    Flist fl;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            fl.addTail(element.get<string>());
        }
    }

    string target;
    string value;
    FNode* node = nullptr;
    switch (command)
    {
        case Command::FPUSHH:
            ss >> value;
            fl.pushFront(value);
            break;
        case Command::FPUSHT:
            ss >> value;
            fl.addTail(value);
            break;
        case Command::FPUSHA:
            ss >> target >> value;
            node = fl.findValue(target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else fl.addOne(node, value);
            break;
        case Command::FPUSHB:
            ss >> target >> value;
            node = fl.findValue(target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else fl.addBefore(node, value);
            break;
        case Command::FDELH:
            fl.popFront();
            break;
        case Command::FDELT:
            fl.popTail();
            break;
        case Command::FDELA:
            ss >> target;
            node = fl.findValue(target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else fl.deleteNode(node);
            break;
        case Command::FDELB:
            ss >> target;
            node = fl.findValue(target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else fl.deleteBefore(node);
            break;
        case Command::FDEL:
            ss >> value;
            fl.deleteValue(value);
            break;
        case Command::FFIND:
            ss >> value;
            if (fl.findValue(value) != nullptr) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::FPRINT:
            fl.printList();
            break;
        case Command::FPRINTR:
            fl.printReverse();
            break;
        default:
            break;
    }

    json data = json::array();
    for (FNode* current = fl.getHead(); current != nullptr; current = current -> next)
    {
        data.push_back(current -> value);
    }
    db[name]["type"] = "flist";
    db[name]["data"] = data;
}
