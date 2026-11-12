#include "queueClass.h"

Queue::Queue()
{
    head = nullptr;
    tail = nullptr;
}

Queue::~Queue()
{
    while (!isEmptyQ())
    {
        pop();
    }
}

bool Queue::isEmptyQ() const
{
    return head == nullptr;
}

void Queue::push(const string& a)
{
    QNode* new_node = new QNode{a, nullptr};

    if (isEmptyQ())
    {
        head = new_node;
        tail = new_node;
    }
    else
    {
        tail -> next = new_node;
        tail = new_node;
    }
}

void Queue::pop()
{
    if (isEmptyQ()) return;

    QNode* tmp = head;
    head = head -> next;
    delete tmp;

    if (isEmptyQ())
    {
        tail = nullptr;
    }
}

string Queue::top() const
{
    if (isEmptyQ()) return "";
    return head -> a;
}

void Queue::printQ() const
{
    QNode* current = head;
    while (current != nullptr)
    {
        cout << current -> a << " -> ";
        current = current -> next;
    }
    cout << "nullptr\n";
}

QNode* Queue::getHead() const
{
    return head;
}

void handleQueue(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "queue")) return;

    Queue q;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            q.push(element.get<string>());
        }
    }

    string value;
    switch (command)
    {
        case Command::QPUSH:
            ss >> value;
            q.push(value);
            cout << "-> " << value << "\n";
            break;
        case Command::QPOP:
            if (q.isEmptyQ()) cout << "-> очередь пуста\n";
            else
            {
                cout << "-> " << q.top() << "\n";
                q.pop();
            }
            break;
        case Command::QPRINT:
            q.printQ();
            break;
        default:
            break;
    }

    json data = json::array();
    for (QNode* current = q.getHead(); current != nullptr; current = current -> next)
    {
        data.push_back(current -> a);
    }
    db[name]["type"] = "queue";
    db[name]["data"] = data;
}

// сериализация: [количество] [строки от головы к хвосту]
void Queue::saveBinary(const string& fileName) const
{
    int count = 0;
    for (QNode* cur = head; cur != nullptr; cur = cur -> next) count++;

    ofstream out(fileName, ios::binary);
    writeInt(out, count);
    for (QNode* cur = head; cur != nullptr; cur = cur -> next)
    {
        writeString(out, cur -> a);
    }
}

void Queue::loadBinary(const string& fileName)
{
    ifstream in(fileName, ios::binary);
    if (!in.is_open()) return;

    while (!isEmptyQ()) pop();                 // очищаем очередь перед загрузкой
    int count = readInt(in);
    for (int i = 0; i < count; i++)
    {
        push(readString(in));
    }
}
