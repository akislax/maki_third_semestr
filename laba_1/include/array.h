#pragma once
#include <iostream>
#include <string>
using namespace std;

struct Array
{
    string* data = nullptr;
    int size = 0;
    int capacity = 0;
};

void initArray(Array* arr);
void resizeArray(Array* arr);
string getAt(Array* arr, int index);
void printArray(Array* arr);
void pushBack(Array* arr, const string& text);
void setAt(Array* arr, const string& text, int index);
int lenghtArray(Array* arr);
void insertAt(Array* arr, const string& text, int index);
void removeAt(Array* arr, int index);
void freeArray(Array* arr);