#include "five_task.h"
#include <iostream>
using namespace std;

HuffNode* takesMin(HuffNode* nodes[], int& count)
{
    int mini = 0;

    for (int i = 1; i < count; i++)
    {
        if (nodes[i] -> weight < nodes[mini] -> weight)
        {
            mini = i;
        }
    }

    HuffNode* result = nodes[mini];
    //запомнили самый маленький

    //затерли
    nodes[mini] = nodes[count - 1];
    count--;

    return result;
}
//count - сколько узлов!!!!!
void HuffTree::buildTree(const string& text)
{
    HuffNode* nodes[256]; //char принимает от 0 до 255 значений поэтому 256 написано

    int count = 0;

    for (char c : text)
    {
        bool found = false;

        for (int i = 0; i < count; i++)
        {
            if (nodes[i] -> symbol == c)
            {
                nodes[i] -> weight++;
                found = true;
                break;
            }
        }

        if (found == false)
        {
            nodes[count] = new HuffNode{c, 1, nullptr, nullptr};
            count ++;
        }
    }

    while (count > 1)
    {
        HuffNode* first = takesMin(nodes, count);
        HuffNode* second = takesMin(nodes, count);

        HuffNode* papa = new HuffNode{'\0', first->weight + second->weight, first, second};
        
        nodes[count] = papa;
        count ++;
    }

    root = nodes[0];
    buildCodes(root, "");
}

//мы используем эту фукнцию как? подаем корень и пустое значение потому что корень ""
void HuffTree::buildCodes(HuffNode* node, string code)
{
    if (node == nullptr) return;

    
    if (node->left == nullptr && node->right == nullptr)
    {
        codes[node->symbol] = code;
        return;
    }

    
    buildCodes(node->left, code + "0");
    buildCodes(node->right, code + "1");
}

//берет из таблицы codes значения и просто выводит результат в виде 0101010
string HuffTree::encode(const string& text)
{
    string result = "";
    for (char c : text)
    {
        result += codes[c];
    }
    return result;
}


//Стоишь в корне.
//Читаешь бит: 0 - шаг влево, 1 - шаг вправо.
//Попал в букву - записал её и вернулся в корень.
string HuffTree::decode(const string& bits)
{
    string result = ""; 
    HuffNode* current = root;     // стоим в корне

    for (char b : bits)
    {
        // шаг по дереву
        if (b == '0')
            current = current->left; //0 levo
        else
            current = current->right;

        // дошли до буквы?
        if (current->left == nullptr && current->right == nullptr)
        {
            result += current->symbol;   // записали букву
            current = root;              // вернулись в корень
        }
    }
    return result;
}

void HuffTree::printTable()
{
    for (int i = 0; i < 256; i++)
    {
        if (codes[i] != "")
        {
            cout << (char)i << " : " << codes[i] << endl;
        }
    }
}

void HuffTree::freeTree(HuffNode* node)
{
    if (node == nullptr) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

HuffTree::~HuffTree()
{
    freeTree(root);
}