#include <iostream>   
#include <string>    
#include <fstream>    
#include <sstream>    
#include "json.hpp"   
#include "stack.h"    

using namespace std;
using json = nlohmann::json;

enum class OptionForMenu
{
    Exit = 0,
    FirstTask = 1,
    SecondTask = 2
};
int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        if (argc != 5)
        {
            cout << "Использование: lab2 --file <файл> --query \"<команда>\"" << endl;
            return 1;
        }
        string fileName = argv[2];
        string query = argv[4];
        stringstream ss(query);
        string cmd, name;
        int value;
        ss >> cmd >> name >> value;

        if (cmd == "SETADD")
        {
            cout << "Добавляю " << value << " в " << name << endl;
        }
        else if (cmd == "SETDEL")
        {
            cout << "Удаляю " << value << " из " << name << endl;
        }
        else if (cmd == "SET_AT")
        {
            cout << "Проверяю, есть ли " << value << " в " << name << endl;
        }
        else
        {
            cout << "Неизвестная команда" << endl;
        }
        return 0; 
    }

    

    OptionForMenu choice;
    do
    {
        cout << "\n1 - Стек\n2 - АТД Множество\n0 - Выход\n> ";

        int input;
        cin >> input;
        cin.ignore();
        choice = static_cast<OptionForMenu>(input);

        switch (choice)
        {
            case OptionForMenu::FirstTask:
            {
                cout << "Введите выражение: ";
                string primer;
                getline(cin, primer);
                cout << schet(primer) << endl;
                break;
            }
            case OptionForMenu::Exit:
            break;
            default:
                cout <<"Нет такого пункта меню\n";
        }
    } while (choice != OptionForMenu::Exit);
   
    return 0;
}