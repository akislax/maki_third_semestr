#include "array_template.h"
#include <iostream>
#include <string>
using namespace std;

template <typename T>
void initArray(Array<T>* arr)
{
    arr -> capacity = 4;
    arr -> data = new T[arr->capacity];
    arr->size = 0;

}

template <typename T>
void resizeArray(Array<T>* arr)
{
    int newCapacity = arr->capacity * 2;
    T* newData = new T [newCapacity];
    for (int i = 0; i < arr->size; i++)
    {
        newData[i] = arr -> data[i];
    }
    delete[] arr->data;
    arr->data = newData;
    arr->capacity = newCapacity;
}

template <typename T>
T getAt(Array<T>* arr, int index)
{
    if (index < 0 || index >= arr -> size)
    {
        cout << "Нет такого индекса\n";
        return T();
    }
    return arr -> data[index];
}

template <typename T>
void printArray(Array<T>* arr)
{
    for (int i = 0; i < arr -> size; i++)
    {
        cout << arr -> data[i] << " ";
    }
     cout << "(size=" << arr->size << ", capacity=" << arr->capacity << ")\n";
}

template <typename T>
void pushBack(Array<T>* arr, const T& text)
{
    if (arr -> size >= arr -> capacity)
    {
        resizeArray(arr);
    }
    arr -> data[arr -> size] = text;
    arr -> size++;
}

template <typename T>
void setAt(Array<T>* arr, const T& text, int index)
{
    if (index <0 || index >= arr -> size)
    {
        cout << "такого индекса нету\n";
        return;
    } 
    arr -> data[index] = text; 
}

template <typename T>
int lenghtArray(Array<T>* arr)
{
    return arr -> size;
}

template <typename T>
void insertAt(Array<T>* arr, const T& text, int index)
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

template <typename T>
void removeAt(Array<T>* arr, int index)
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

template <typename T>
void freeArray(Array<T>* arr)
{
    delete [] arr -> data;
    arr -> data = nullptr;
    arr -> size = 0;
    arr -> capacity = 0;
}

template void initArray<int>(Array<int>*);
template void resizeArray<int>(Array<int>*);
template int getAt<int>(Array<int>*, int);
template void printArray<int>(Array<int>*);
template void pushBack<int>(Array<int>*, const int&);
template void setAt<int>(Array<int>*, const int&, int);
template int lenghtArray<int>(Array<int>*);
template void insertAt<int>(Array<int>*, const int&, int);
template void removeAt<int>(Array<int>*, int);
template void freeArray<int>(Array<int>*);

template void initArray<string>(Array<string>*);
template void resizeArray<string>(Array<string>*);
template string getAt<string>(Array<string>*, int);
template void printArray<string>(Array<string>*);
template void pushBack<string>(Array<string>*, const string&);
template void setAt<string>(Array<string>*, const string&, int);
template int lenghtArray<string>(Array<string>*);
template void insertAt<string>(Array<string>*, const string&, int);
template void removeAt<string>(Array<string>*, int);
template void freeArray<string>(Array<string>*);