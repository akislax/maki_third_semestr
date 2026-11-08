#include "third_task.h"
#include "second_task.h"
#include "array_template.h"
#include <iostream>
using namespace std;

void similarSubsets(hashTable<int>& set)
{
    Array<int> arr;
    initArray(&arr);
    set.toArray(arr);
    int n = arr.size;

    int bestMask = 0;
    int bestDiff = -1;

    int total = 1;
    for (int i = 0; i < n; i++)
    {
        total = total * 2;
    }

    for (int mask = 0; mask < total; mask++)
    {
        int left = 0;
        int right = 0;
        int m = mask;

        for (int i = 0; i < n; i++)
        {
            if (m % 2 == 1)
                left += arr.data[i];
            else
                right += arr.data[i];

            m = m / 2;
        }

        int diff = abs(left - right);

        if (bestDiff == -1 || diff < bestDiff)
        {
            bestDiff = diff;
            bestMask = mask;
        }
    }

    int m = bestMask;
    cout << "Первое подмножество: { ";
    for (int i = 0; i < n; i++)
    {
        if (m % 2 == 1)
            cout << arr.data[i] << " ";
        m = m / 2;
    }
    cout << "}" << endl;

    m = bestMask;
    cout << "Второе подмножество: { ";
    for (int i = 0; i < n; i++)
    {
        if (m % 2 == 0)
            cout << arr.data[i] << " ";
        m = m / 2;
    }
    cout << "}" << endl;

    cout << "Разница сумм: " << bestDiff << endl;

    freeArray(&arr);
}   
