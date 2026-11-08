#include "fourth_task.h"
#include "array_template.h"
#include <iostream>
using namespace std;


void goodLine(Array<int>& arr, int userInt)
{
    int m = lenghtArray(&arr);

    for (int i = 0; i < m; i++)
    {
        int summa = 0;
        for (int j = i; j < m; j++)
        {
            summa += arr.data[j];

            if (summa == userInt)
            {
                cout << "[ ";
                for (int k = i; k <= j; k++)
                {
                    cout << arr.data[k] << " ";
                }
                cout << "]" << endl;
            }
        }
    }
}