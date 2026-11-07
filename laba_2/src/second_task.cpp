#include "laba_2\include\second_tack.h"
#include <string>
using namespace std;

const double a = 0.6180339887;
int hashkey(const string& key)
{
    unsigned long h = 0;
    for (char c : key) h = h * 31 + c;
    return h % 100;
}

int hashkey(int key)
{
    double x = key * a;
    double sda = x - int(x);
    if (sda < 0)
    {
        sda +=1;
    }
    int h = int (100 * sda);
    return h;
}

template <typename T>
int hashTable<T>::hashFunc(const T& key)
{
    int beta = hashkey(key) % SIZE;
    return beta;
}

template <typename T>
bool hashTable<T>::search(const T& key)
{
    int index = hashFunc(key);
    Node<T>* current = table[index];
    while(current != nullptr)
    {
        if (current -> value == key)
        {
            return true;
        }
        current = current -> next;
    }

    return false;
}

template <typename T>
void hashTable<T>::add(const T& key)
{
    if (search)
}