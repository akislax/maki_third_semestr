#include "array.h"
#include "flist.h"
#include "dlist.h"
#include "stack.h"
#include "queue.h"
#include "tree.h"
#include <sstream>
#include <fstream>

string stackToString(Stack* stack)
{
    string result = "";
    SNode* current = stack -> head;
    while (current != nullptr)
    {
        if (result == "") result = current -> a;
        else result = current -> a + " " + result;
        current = current -> next;
    }
    return result;
}

string queueToString(Q* q)
{
    string result = "";
    QNode* current = q -> head;
    while (current != nullptr)
    {
        if (result == "") result = current -> a;
        else result = result + " " + current -> a;
        current = current -> next;
    }
    return result;
}

string arrayToString(Array* arr)
{
    string result = "";
    for (int i = 0; i < arr -> size; i++)
    {
        if (result == "") result = arr -> data[i];
        else result = result + " " + arr -> data[i];
    }
    return result;
}

string flistToString(FNode* head)
{
    string result = "";
    FNode* current = head;
    while (current != nullptr)
    {
        if (result == "") result = current -> value;
        else result = result + " " + current -> value;
        current = current -> next;
    }
    return result;
}

string dlistToString(Dlist* dlist)
{
    string result = "";
    DNode* current = dlist -> head;
    while (current != nullptr)
    {
        if (result == "") result = current -> person;
        else result = result + " " + current -> person;
        current = current -> next;
    }
    return result;
}

string treeToString(tree* root)
{
    string result = "";
    if (root == nullptr) return result;

    vector<tree*> ochered;
    ochered.push_back(root);
    for (size_t i = 0; i < ochered.size(); i++)
    {
        tree* node = ochered[i];
        if (result == "") result = to_string(node -> key);
        else result = result + " " + to_string(node -> key);
        if (node -> left != nullptr) ochered.push_back(node -> left);
        if (node -> right != nullptr) ochered.push_back(node -> right);
    }
    return result;
}

int main(int argc, char* argv[]) //сколько слов в команде __ сами слова
{
    if (argc != 5)
    {
        cout << "Использование: ./dbms --file <файл> --query '<команда>'\n";
        return 1;
    }
    string fileName = argv[2];
    string query = argv[4];

    //грубо говоря это строки для работы с каждой стуктуры
    string lines[6];
    ifstream in(fileName);
    for (int i = 0; i < 6; i++)
    {
        getline(in, lines[i]);
    }
    in.close();

    //это чтение команды. что мы используем
    stringstream ss(query);
    string cmd;
    ss >> cmd; // читается только одно слова. SPUSH 5. Будет лежать только SPUSH

    string element;

    Array arr;
    initArray(&arr);
    stringstream al(lines[0]);
    while (al >> element)
    {
        pushBack(&arr, element);
    }

    FNode* fhead = nullptr;
    stringstream fl(lines[1]);
    while (fl >> element)
    {
        fhead = addTail(fhead, element);
    }

    Dlist dl;
    stringstream dll(lines[2]);
    while (dll >> element)
    {
        pushBack(&dl, element);
    }

    Stack stack;
    stringstream s1(lines[3]);
    while (s1 >> element)
    {
        push(&stack, element);
    }

    Q q;
    stringstream ql(lines[4]);
    while (ql >> element)
    {
        push(&q, element);
    }

    tree* root = nullptr;
    stringstream tl(lines[5]);
    int key;
    while (tl >> key)
    {
        root = insert(root, key);
    }

    if (cmd == "MPUSH")
    {
        string value;
        ss >> value;
        pushBack(&arr, value);
        cout << "-> " << value << "\n";
    }
    else if (cmd == "MINSERT")
    {
        int index;
        string value;
        ss >> index >> value;
        insertAt(&arr, value, index);
    }
    else if (cmd == "MGET")
    {
        int index;
        ss >> index;
        cout << "-> " << getAt(&arr, index) << "\n";
    }
    else if (cmd == "MSET")
    {
        int index;
        string value;
        ss >> index >> value;
        setAt(&arr, value, index);
    }
    else if (cmd == "MDEL")
    {
        int index;
        ss >> index;
        removeAt(&arr, index);
    }
    else if (cmd == "MLEN")
    {
        cout << "-> " << lenghtArray(&arr) << "\n";
    }
    else if (cmd == "MPRINT")
    {
        printArray(&arr);
    }
    else if (cmd == "FPUSHH")
    {
        string value;
        ss >> value;
        fhead = pushFront(fhead, value);
    }
    else if (cmd == "FPUSHT")
    {
        string value;
        ss >> value;
        fhead = addTail(fhead, value);
    }
    else if (cmd == "FPUSHA")
    {
        string target, value;
        ss >> target >> value;
        FNode* node = findValue(fhead, target);
        if (node == nullptr) cout << "нет такого элемента\n";
        else addOne(node, value);
    }
    else if (cmd == "FPUSHB")
    {
        string target, value;
        ss >> target >> value;
        FNode* node = findValue(fhead, target);
        if (node == nullptr) cout << "нет такого элемента\n";
        else fhead = addBefore(fhead, node, value);
    }
    else if (cmd == "FDELH")
    {
        fhead = popFront(fhead);
    }
    else if (cmd == "FDELT")
    {
        fhead = popTail(fhead);
    }
    else if (cmd == "FDELA")
    {
        string target;
        ss >> target;
        FNode* node = findValue(fhead, target);
        if (node == nullptr) cout << "нет такого элемента\n";
        else deleteNode(node);
    }
    else if (cmd == "FDELB")
    {
        string target;
        ss >> target;
        FNode* node = findValue(fhead, target);
        if (node == nullptr) cout << "нет такого элемента\n";
        else fhead = deleteBefore(fhead, node);
    }
    else if (cmd == "FDEL")
    {
        string value;
        ss >> value;
        fhead = deleteValue(fhead, value);
    }
    else if (cmd == "FFIND")
    {
        string value;
        ss >> value;
        if (findValue(fhead, value) != nullptr) cout << "-> TRUE\n";
        else cout << "-> FALSE\n";
    }
    else if (cmd == "FPRINT")
    {
        printList(fhead);
    }
    else if (cmd == "FPRINTR")
    {
        printReverse(fhead);
        cout << "\n";
    }
    else if (cmd == "LPUSHH")
    {
        string value;
        ss >> value;
        pushFront(&dl, value);
    }
    else if (cmd == "LPUSHT")
    {
        string value;
        ss >> value;
        pushBack(&dl, value);
    }
    else if (cmd == "LPUSHA")
    {
        string target, value;
        ss >> target >> value;
        DNode* node = findName(&dl, target);
        if (node == nullptr) cout << "нет такого элемента\n";
        else pushBeetwen(node, &dl, value);
    }
    else if (cmd == "LPUSHB")
    {
        string target, value;
        ss >> target >> value;
        DNode* node = findName(&dl, target);
        if (node == nullptr) cout << "нет такого элемента\n";
        else pushBefore(node, &dl, value);
    }
    else if (cmd == "LDELH")
    {
        popFront(&dl);
    }
    else if (cmd == "LDELT")
    {
        popBack(&dl);
    }
    else if (cmd == "LDELA")
    {
        string target;
        ss >> target;
        DNode* node = findName(&dl, target);
        if (node == nullptr) cout << "нет такого элемента\n";
        else popAfter(node, &dl);
    }
    else if (cmd == "LDELB")
    {
        string target;
        ss >> target;
        DNode* node = findName(&dl, target);
        if (node == nullptr) cout << "нет такого элемента\n";
        else popBefore(node, &dl);
    }
    else if (cmd == "LDEL")
    {
        string value;
        ss >> value;
        popName(&dl, value);
    }
    else if (cmd == "LFIND")
    {
        string value;
        ss >> value;
        if (findName(&dl, value) != nullptr) cout << "-> TRUE\n";
        else cout << "-> FALSE\n";
    }
    else if (cmd == "LPRINT")
    {
        printForward(&dl);
    }
    else if (cmd == "LPRINTR")
    {
        printBackward(&dl);
    }
    else if (cmd == "SPUSH")
    {
        string value;
        ss >> value;
        push(&stack, value);
        cout << "-> " << value << "\n";
    }
    else if (cmd == "SPOP")
    {
        if (isEmptyStack(&stack))
        {
            cout << "-> стек пуст\n";
        }
        else
        {
            cout << "-> " << top(&stack) << "\n";
            pop(&stack);
        }
    }
    else if (cmd == "SPRINT")
    {
        print(&stack);
    }
    else if (cmd == "QPUSH")
    {
        string value;
        ss >> value;
        push(&q, value);
        cout << "-> " << value << "\n";
    }
    else if (cmd == "QPOP")
    {
        if (isEmptyQ(&q))
        {
            cout << "-> очередь пуста\n";
        }
        else
        {
            cout << "-> " << top(&q) << "\n";
            pop(&q);
        }
    }
    else if (cmd == "QPRINT")
    {
        printQ(&q);
    }
    else if (cmd == "TINSERT")
    {
        int value;
        ss >> value;
        root = insert(root, value);
        cout << "-> " << value << "\n";
    }
    else if (cmd == "TFIND")
    {
        int value;
        ss >> value;
        if (search(root, value) != nullptr) cout << "-> TRUE\n";
        else cout << "-> FALSE\n";
    }
    else if (cmd == "TCOMPLETE")
    {
        if (isComplete(root)) cout << "-> TRUE\n";
        else cout << "-> FALSE\n";
    }
    else if (cmd == "TPRINT")
    {
        cout << "прямой:       "; preorder(root);  cout << "\n";
        cout << "симметричный: "; inorder(root);   cout << "\n";
        cout << "обратный:     "; postorder(root); cout << "\n";
        cout << "в ширину:     "; bfs(root);       cout << "\n";
    }
    else if (cmd == "PRINT")
    {
        string which;
        ss >> which;
        if (which == "M") printArray(&arr);
        else if (which == "F") printList(fhead);
        else if (which == "L") printForward(&dl);
        else if (which == "S") print(&stack);
        else if (which == "Q") printQ(&q);
        else if (which == "T")
        {
            bfs(root);
            cout << "\n";
        }
        else cout << "укажи структуру: M, F, L, S, Q или T\n";
    }
    else
    {
        cout << "неизвестная команда\n";
    }

    lines[0] = arrayToString(&arr);
    lines[1] = flistToString(fhead);
    lines[2] = dlistToString(&dl);
    lines[3] = stackToString(&stack);
    lines[4] = queueToString(&q);
    lines[5] = treeToString(root);

    ofstream out(fileName);
    for (int i = 0; i < 6; i++)
    {
        out << lines[i] << "\n";
    }
    out.close();

    freeArray(&arr);
    freeList(fhead);
    freeList(&dl);
    freeStack(&stack);
    freeQ(&q);
    freeTree(root);

    return 0;
}