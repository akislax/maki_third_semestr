#include <iostream>
#include <string>
using namespace std;

struct Array
{
    string* data = nullptr;
    int size = 0;
    int capacity = 0;
};

void initArray(Array* arr)
{
    arr -> capacity = 4;
    arr -> data = new string[arr->capacity];
    arr->size = 0;

}
void resizeArray(Array* arr)
{
    int newCapacity = arr->capacity * 2;
    string* newData = new string [newCapacity];
    for (int i = 0; i < arr->size; i++)
    {
        newData[i] = arr -> data[i];
    }
    delete[] arr->data;
    arr->data = newData;
    arr->capacity = newCapacity;
}

string getAt(Array* arr, int index)
{
    if (index <0 || index >= arr -> size)
    {
        cout << "Нет такого индекса\n";
        return "";
    }
    return arr -> data[index];
}

void printArray(Array* arr)
{
    for (int i = 0; i < arr -> size; i++)
    {
        cout << arr -> data[i] << " ";
    }
     cout << "(size=" << arr->size << ", capacity=" << arr->capacity << ")\n";
}

void pushBack(Array* arr, const string& text)
{
    if (arr -> size >= arr -> capacity)
    {
        resizeArray(arr);
    }
    arr -> data[arr -> size] = text;
    arr -> size++;
}

void setAt(Array* arr, const string& text, int index)
{
    if (index <0 || index >= arr -> size)
    {
        cout << "такого индекса нету\n";
        return;
    } 
    arr -> data[index] = text; 
}

int lenghtArray(Array* arr)
{
    return arr -> size;
}

void insertAt(Array* arr, const string& text, int index)
{
     if (index < 0 || index > arr -> size)
    {
        cout << "такого индекса нету\n";
        return;
    } 

    if (arr->size == arr->capacity)
    {
        resizeArray(arr);
    }

    // Первым пересаживается последний: справа от него свободно.
    // Если начать с начала — затрём соседа справа раньше, чем он успеет пересесть.
    //
    // Пример: a b c d, вставляем X на место 1
    //   i = 3: data[4] = data[3]  ->  a b c d d
    //   i = 2: data[3] = data[2]  ->  a b c c d
    //   i = 1: data[2] = data[1]  ->  a b b c d
    //   i = 0: 0 >= 1 неверно, цикл закончился
    for (int i = arr -> size - 1 ; i >= index; i--)
    {
        arr->data[i + 1] = arr->data[i];
    }
    
    arr->data[index] = text;
    arr->size++;

}

void removeAt(Array* arr, int index)
{
    
    if (index < 0 || index >= arr->size)
    {
        cout << "такого индекса нету\n";
        return;
    }

    
    // Первым пересаживается сосед справа от дырки, потом следующий и т.д.
    // Удаляемый элемент затирается сам на первом же шаге.
    // До последнего элемента не идём: справа от него брать нечего.
    // Пример: a X b c d, удаляем место 1 (size = 5)
    //   i = 1: data[1] = data[2]  ->  a b b c d
    //   i = 2: data[2] = data[3]  ->  a b c c d
    //   i = 3: data[3] = data[4]  ->  a b c d d
    //   i = 4: 4 < 4 неверно, цикл закончился
    for (int i = index; i < arr->size - 1; i++)
    {
        arr->data[i] = arr->data[i + 1];   // элемент с места i+1 переезжает на место i
    }

    arr->size--;
}

void freeArray(Array* arr)
{
    delete [] arr -> data;
    arr -> data = nullptr;
    arr -> size = 0;
    arr -> capacity = 0;
}
int main()
{
    Array arr;
    initArray(&arr);
    insertAt(&arr, "a", 0);
    insertAt(&arr, "b", 1);
    insertAt(&arr, "c", 2);
    printArray(&arr);
    insertAt(&arr, "X", 1);
printArray(&arr);    // a X b c (size=4, capacity=4)
insertAt(&arr, "Y", 1);
printArray(&arr);    // a X b c (size=4, capacity=4)
    return 0;
}