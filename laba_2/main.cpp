#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "first_task.h"
#include "second_task.h"
#include "third_task.h"
#include "fourth_task.h"
#include "five_task.h"
#include "six_task.h"
#include "seven_task.h"

using json = nlohmann::json;
using namespace std;

enum class OptionForMenu
{
    Exit = 0,
    FirstTask = 1,
    SecondTask = 2,
    ThirdTask = 3,
    FourthTask = 4,
    FiveTask = 5,
    SixOneTask = 6,
    SixTwoTask = 7,
    CompareTask = 8,
    SevenTask = 9
};

int main()
{
    OptionForMenu choice;
    do
    {
        cout << "\n1 - Стек\n2 - АТД Множество\n3 - Множество\n4 - Массив\n5 - Хаффман\n"
             << "6 - Хеш-таблица (подзадание 6.1)\n7 - Хеш-таблица (подзадание 6.2)\n"
             << "8 - Сравнение кукушки и цепочек\n9 - Кэш\n0 - Выход\n> ";

        int input;
        if (!(cin >> input))
        {
            if (cin.eof()) return 0;      // ввод закончился - выходим
            cin.clear();                  // ввели не число - чиним cin
            input = -1;                   // и считаем, что такого пункта нет
        }
        cin.ignore(10000, '\n');          // убираем остаток строки вместе с Enter
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
            else if (cmd == "SET_AT")
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
            getline(cin, line);

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
            getline(cin, line);

            stringstream ss(line);
            Array<int> arr;
            initArray(&arr);

            int x;
            while (ss >> x)
            {
                pushBack(&arr, x);
            }

            cout << "Введите ваше число: ";
            string numLine;
            getline(cin, numLine);
            stringstream numSs(numLine);
            int userInt;
            if (!(numSs >> userInt))
            {
                cout << "Нужно целое число" << endl;
                freeArray(&arr);
                break;
            }

            goodLine(arr, userInt);
            freeArray(&arr);
            break;
        }
        case OptionForMenu::FiveTask:
        {
            cout << "Введите строку: ";
            string text;
            getline(cin, text);

            if (text.empty())
            {
                cout << "Пустая строка" << endl;
                break;
            }

            HuffTree h;
            h.buildTree(text);

            string bits = h.encode(text);
            cout << "Код: " << bits << endl;

            cout << "Таблица:" << endl;
            h.printTable();

            cout << "Обратно: " << h.decode(bits) << endl;
            break;
        }
        case OptionForMenu::SixOneTask:
        {
            cout << "1 - Метод цепочек\n2 - Хеширование кукушки\n> ";
            string typeLine;
            getline(cin, typeLine);
            stringstream typeSs(typeLine);
            int type = 0;
            typeSs >> type;

            if (type != 1 && type != 2)
            {
                cout << "Нет такой таблицы" << endl;
                break;
            }

            hashTableSix chain;
            Kukushka cuckoo;

            cout << "Команды: ADD x, DEL x, FIND x, EXIT" << endl;

            while (true)
            {
                cout << "> ";
                string line;
                if (!getline(cin, line)) break;   // ввод закончился

                stringstream ss(line);
                string cmd;
                ss >> cmd;

                if (cmd == "EXIT")
                {
                    break;
                }

                if (cmd != "ADD" && cmd != "DEL" && cmd != "FIND")
                {
                    cout << "Неизвестная команда" << endl;
                    continue;
                }

                int x;
                if (!(ss >> x))
                {
                    cout << "Нужно целое число" << endl;
                    continue;
                }

                if (cmd == "ADD")
                {
                    if (type == 1)
                        chain.add(x);
                    else
                        cuckoo.insert(x);
                    cout << "Добавлено" << endl;
                }
                else if (cmd == "DEL")
                {
                    if (type == 1)
                        chain.remove(x);
                    else
                        cuckoo.remove(x);
                    cout << "Удалено" << endl;
                }
                else
                {
                    bool found;
                    if (type == 1)
                        found = chain.search(x);
                    else
                        found = cuckoo.search(x);
                    cout << (found ? "есть" : "нет") << endl;
                }
            }
            break;
        }
        case OptionForMenu::SixTwoTask:
        {
            cout << "Введите число: ";
            string numLine;
            getline(cin, numLine);
            stringstream numSs(numLine);
            int x;
            if (!(numSs >> x))
            {
                cout << "Нужно целое число" << endl;
                break;
            }

            printFold(x);
            break;
        }
        case OptionForMenu::CompareTask:
        {
            compareHash();
            break;
        }
        case OptionForMenu::SevenTask:
        {
            cout << "Ёмкость кэша: ";
            int cap;
            cin >> cap;

            cout << "Количество запросов: ";
            int q;
            cin >> q;

            if (!cin || cap <= 0 || q < 0)
            {
                cout << "Нужны положительные целые числа" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }

            LRUCache cache(cap);

            cout << "Запросы: ";
            for (int i = 0; i < q; i++)
            {
                string cmd;
                cin >> cmd;

                if (cmd == "SET")
                {
                    int x, y;
                    cin >> x >> y;
                    cache.set(x, y);
                }
                else if (cmd == "GET")
                {
                    int x;
                    cin >> x;
                    cout << cache.get(x) << " ";
                }
            }
            cout << endl;
            cin.clear();
            cin.ignore(10000, '\n');
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