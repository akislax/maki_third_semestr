#pragma once
#include <iostream>
#include <string>
using namespace std;

template <typename T>
struct Array
{
    T* data = nullptr;
    int size = 0;
    int capacity = 0;
};

template <typename T>
void initArray(Array<T>* arr);

template <typename T>
void resizeArray(Array<T>* arr);

template <typename T>
T getAt(Array<T>* arr, int index);

template <typename T>
void printArray(Array<T>* arr);

template <typename T>
void pushBack(Array<T>* arr, const T& text);

template <typename T>
void setAt(Array<T>* arr, const T& text, int index);

template <typename T>
int lenghtArray(Array<T>* arr);

template <typename T>
void insertAt(Array<T>* arr, T& text, int index);

template <typename T>
void removeAt(Array<T>* arr, int index);
template <typename T>
void freeArray(Array<T>* arr);
