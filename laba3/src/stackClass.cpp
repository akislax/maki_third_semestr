#include "stackClass.h"

Stack::Stack()
{
    head = nullptr;
}

Stack::~Stack()
{
    while (!isEmptyStack())
    {
        pop();
    }
}

bool Stack::isEmptyStack() const
{
    return head == nullptr;
}

void Stack::push(const string& a)
{
    SNode* new_node = new SNode{a, head};
    head = new_node;
}

void Stack::pop()
{
    if (isEmptyStack()) return;
    SNode* tmp = head;
    head = head -> next;
    delete tmp;
}

string Stack::top() const
{
    if (isEmptyStack()) return "";
    return head -> a;
}

void Stack::print() const
{
    SNode* current = head;
    while (current != nullptr)
    {
        cout << current -> a << " -> ";
        current = current -> next;
    }
    cout << "nullptr\n";
}

SNode* Stack::getHead() const
{
    return head;
}

void handleStack(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "stack")) return;

    Stack stack;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            stack.push(element.get<string>());
        }
    }

    string value;
    switch (command)
    {
        case Command::SPUSH:
            ss >> value;
            stack.push(value);
            cout << "-> " << value << "\n";
            break;
        case Command::SPOP:
            if (stack.isEmptyStack()) cout << "-> стек пуст\n";
            else
            {
                cout << "-> " << stack.top() << "\n";
                stack.pop();
            }
            break;
        case Command::SPRINT:
            stack.print();
            break;
        default:
            break;
    }

    // идем от вершины вниз и вставляем в начало, чтобы в json был порядок "снизу вверх"
    json data = json::array();
    for (SNode* current = stack.getHead(); current != nullptr; current = current -> next)
    {
        data.insert(data.begin(), current -> a);
    }
    db[name]["type"] = "stack";
    db[name]["data"] = data;
}
