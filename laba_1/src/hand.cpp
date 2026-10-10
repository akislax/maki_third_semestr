#include "hand.h"
#include "array.h"
#include "flist.h"
#include "dlist.h"
#include "stack.h"
#include "queue.h"
#include "tree.h"

bool wrongType(json& db, const string& name, const string& type)
{
    if (db.contains(name) && db[name]["type"] != type.c_str())
    {
        cout << name << " это не " << type << "\n";
        return true;
    }
    return false;
}

void handleArray(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "array")) return;

    Array arr;
    initArray(&arr);
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            pushBack(&arr, element.get<string>());
        }
    }

    int index;
    string value;
    switch (command)
    {
        case Command::MPUSH:
            ss >> value;
            pushBack(&arr, value);
            cout << "-> " << value << "\n";
            break;
        case Command::MINSERT:
            ss >> index >> value;
            insertAt(&arr, value, index);
            break;
        case Command::MGET:
            ss >> index;
            cout << "-> " << getAt(&arr, index) << "\n";
            break;
        case Command::MSET:
            ss >> index >> value;
            setAt(&arr, value, index);
            break;
        case Command::MDEL:
            ss >> index;
            removeAt(&arr, index);
            break;
        case Command::MLEN:
            cout << "-> " << lenghtArray(&arr) << "\n";
            break;
        case Command::MPRINT:
            printArray(&arr);
            break;
        default:
            break;
    }

    json data = json::array();
    for (int i = 0; i < arr.size; i++)
    {
        data.push_back(arr.data[i]);
    }
    db[name]["type"] = "array";
    db[name]["data"] = data;

    freeArray(&arr);
}

void handleFlist(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "flist")) return;

    FNode* head = nullptr;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            head = addTail(head, element.get<string>());
        }
    }

    string target, value;
    FNode* node = nullptr;
    switch (command)
    {
        case Command::FPUSHH:
            ss >> value;
            head = pushFront(head, value);
            break;
        case Command::FPUSHT:
            ss >> value;
            head = addTail(head, value);
            break;
        case Command::FPUSHA:
            ss >> target >> value;
            node = findValue(head, target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else addOne(node, value);
            break;
        case Command::FPUSHB:
            ss >> target >> value;
            node = findValue(head, target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else head = addBefore(head, node, value);
            break;
        case Command::FDELH:
            head = popFront(head);
            break;
        case Command::FDELT:
            head = popTail(head);
            break;
        case Command::FDELA:
            ss >> target;
            node = findValue(head, target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else deleteNode(node);
            break;
        case Command::FDELB:
            ss >> target;
            node = findValue(head, target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else head = deleteBefore(head, node);
            break;
        case Command::FDEL:
            ss >> value;
            head = deleteValue(head, value);
            break;
        case Command::FFIND:
            ss >> value;
            if (findValue(head, value) != nullptr) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::FPRINT:
            printList(head);
            break;
        case Command::FPRINTR:
            printReverse(head);
            cout << "\n";
            break;
        default:
            break;
    }

    json data = json::array();
    for (FNode* current = head; current != nullptr; current = current -> next)
    {
        data.push_back(current -> value);
    }
    db[name]["type"] = "flist";
    db[name]["data"] = data;

    freeList(head);
}

void handleDlist(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "dlist")) return;

    Dlist dl;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            pushBack(&dl, element.get<string>());
        }
    }

    string target, value;
    DNode* node = nullptr;
    switch (command)
    {
        case Command::LPUSHH:
            ss >> value;
            pushFront(&dl, value);
            break;
        case Command::LPUSHT:
            ss >> value;
            pushBack(&dl, value);
            break;
        case Command::LPUSHA:
            ss >> target >> value;
            node = findName(&dl, target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else pushBeetwen(node, &dl, value);
            break;
        case Command::LPUSHB:
            ss >> target >> value;
            node = findName(&dl, target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else pushBefore(node, &dl, value);
            break;
        case Command::LDELH:
            popFront(&dl);
            break;
        case Command::LDELT:
            popBack(&dl);
            break;
        case Command::LDELA:
            ss >> target;
            node = findName(&dl, target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else popAfter(node, &dl);
            break;
        case Command::LDELB:
            ss >> target;
            node = findName(&dl, target);
            if (node == nullptr) cout << "нет такого элемента\n";
            else popBefore(node, &dl);
            break;
        case Command::LDEL:
            ss >> value;
            popName(&dl, value);
            break;
        case Command::LFIND:
            ss >> value;
            if (findName(&dl, value) != nullptr) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::LPRINT:
            printForward(&dl);
            break;
        case Command::LPRINTR:
            printBackward(&dl);
            break;
        default:
            break;
    }

    json data = json::array();
    for (DNode* current = dl.head; current != nullptr; current = current -> next)
    {
        data.push_back(current -> person);
    }
    db[name]["type"] = "dlist";
    db[name]["data"] = data;

    freeList(&dl);
}

void handleStack(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "stack")) return;

    Stack stack;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            push(&stack, element.get<string>());
        }
    }

    string value;
    switch (command)
    {
        case Command::SPUSH:
            ss >> value;
            push(&stack, value);
            cout << "-> " << value << "\n";
            break;
        case Command::SPOP:
            if (isEmptyStack(&stack)) cout << "-> стек пуст\n";
            else
            {
                cout << "-> " << top(&stack) << "\n";
                pop(&stack);
            }
            break;
        case Command::SPRINT:
            print(&stack);
            break;
        default:
            break;
    }

    json data = json::array();
    for (SNode* current = stack.head; current != nullptr; current = current -> next)
    {
        data.insert(data.begin(), current -> a);
    }
    db[name]["type"] = "stack";
    db[name]["data"] = data;

    freeStack(&stack);
}

void handleQueue(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "queue")) return;

    Q q;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            push(&q, element.get<string>());
        }
    }

    string value;
    switch (command)
    {
        case Command::QPUSH:
            ss >> value;
            push(&q, value);
            cout << "-> " << value << "\n";
            break;
        case Command::QPOP:
            if (isEmptyQ(&q)) cout << "-> очередь пуста\n";
            else
            {
                cout << "-> " << top(&q) << "\n";
                pop(&q);
            }
            break;
        case Command::QPRINT:
            printQ(&q);
            break;
        default:
            break;
    }

    json data = json::array();
    for (QNode* current = q.head; current != nullptr; current = current -> next)
    {
        data.push_back(current -> a);
    }
    db[name]["type"] = "queue";
    db[name]["data"] = data;

    freeQ(&q);
}

void handleTree(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "tree")) return;

    tree* root = nullptr;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            root = insert(root, element.get<int>());
        }
    }

    int value;
    switch (command)
    {
        case Command::TINSERT:
            ss >> value;
            root = insert(root, value);
            cout << "-> " << value << "\n";
            break;
        case Command::TFIND:
            ss >> value;
            if (search(root, value) != nullptr) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::TCOMPLETE:
            if (isComplete(root)) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::TPRINT:
            cout << "прямой:       "; preorder(root);  cout << "\n";
            cout << "симметричный: "; inorder(root);   cout << "\n";
            cout << "обратный:     "; postorder(root); cout << "\n";
            cout << "в ширину:     "; bfs(root);       cout << "\n";
            break;
        default:
            break;
    }

    json data = json::array();
    vector<tree*> ochered;
    if (root != nullptr) ochered.push_back(root);
    for (size_t i = 0; i < ochered.size(); i++)
    {
        tree* node = ochered[i];
        data.push_back(node -> key);
        if (node -> left != nullptr) ochered.push_back(node -> left);
        if (node -> right != nullptr) ochered.push_back(node -> right);
    }
    db[name]["type"] = "tree";
    db[name]["data"] = data;

    freeTree(root);
}

void handlePrint(json& db, const string& name, stringstream& ss)
{
    if (!db.contains(name))
    {
        cout << "нет структуры " << name << "\n";
        return;
    }

    if (db[name]["type"] == "array") handleArray(Command::MPRINT, db, name, ss);
    else if (db[name]["type"] == "flist") handleFlist(Command::FPRINT, db, name, ss);
    else if (db[name]["type"] == "dlist") handleDlist(Command::LPRINT, db, name, ss);
    else if (db[name]["type"] == "stack") handleStack(Command::SPRINT, db, name, ss);
    else if (db[name]["type"] == "queue") handleQueue(Command::QPRINT, db, name, ss);
    else if (db[name]["type"] == "tree") handleTree(Command::TPRINT, db, name, ss);
}