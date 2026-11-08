#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "first_task.h"
#include "second_task.h"
#include "third_task.h"
#include "fourth_task.h"

using json = nlohmann::json;
using namespace std;

enum class OptionForMenu
{
    Exit = 0,
    FirstTask = 1,
    SecondTask = 2,
    ThirdTask = 3,
    FourthTask = 4
};

int main()
{
    OptionForMenu choice;
    do
    {
        cout << "\n1 - Стек\n2 - АТД Множество\n3 - Множествоn\n4 - Массив\n0 - Выход\n>";

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
            case OptionForMenu::SecondTask:
            {
                const string fileName = "data.json";

                cout << "Введите команду: ";
                string query;
                getline(cin, query);

                stringstream ss(query);
                string cmd, name, value;
                ss >> cmd >> name >> value;

        
                json db = json::object();
                ifstream in(fileName);
                if (in.is_open() && in.peek() != EOF)
                {
                    db = json::parse(in);
                }
                in.close();

                hashTable<string> set;
                if (db.contains(name))
                {
                    set.loadFromJson(db[name]);
                }

                if (cmd == "SETADD")
                {
                    set.add(value);
                    cout << "Добавлено" << endl;
                }
                else if (cmd == "SETDEL")
                {
                    set.remove(value);
                    cout << "Удалено" << endl;
                }
                else if (cmd == "SET_toArrAT")
                {
                    cout << (set.search(value) ? "TRUE" : "FALSE") << endl;
                }
                else
                {
                    cout << "Неизвестная команда" << endl;
                    break;
                }

                db[name] = set.saveToJson();
                ofstream out(fileName);
                out << db.dump(4) << endl;
                out.close();
                break;
            }
            case OptionForMenu::ThirdTask:
            {
                cout << "Введите числа через пробел: ";
                string line;
                getline (cin,line);

                stringstream ss(line);
                hashTable<int> set;
                int x;
                while (ss >> x)
                {
                    set.add(x);
                }

                similarSubsets(set);
                break;
            }
            case OptionForMenu::FourthTask:
            {
                cout << "Введите числа через пробел: ";
                string line;
                getline(cin,line);
                
                stringstream ss(line);
                Array<int> set;
                initArray(&set);

                int x;
                while(ss >> x)
                {
                    pushBack(&set, x);

                }

                cout << "Введите ваше число: ";
                int userInt;
                cin >> userInt;
                cin.ignore();

                goodLine(set, userInt);
                freeArray(&set);
                break;
            }

            case OptionForMenu::Exit:
                break;
            default:
                cout << "Нет такого пункта меню\n";
        }
    } while (choice != OptionForMenu::Exit);

    return 0;
}