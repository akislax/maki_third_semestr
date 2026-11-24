#include <iostream>
#include "HashTable.h"
#include "Queue.h"
#include "Set.h"
#include "Stack.h"
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <string>
#include <sstream>
#include "Commands.h"
#include "Handlers.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 5 || string(argv[1]) != "--file" || string(argv[3]) != "--query")
    {
        cout << "Использование: ./maki --file <файл> --query '<команда>'\n";
        return 1;
    }

    string fileName = argv[2];
    string query = argv[4];

    json db = json::object();

    ifstream in(fileName);

    if (in.is_open() && in.peek() != EOF)
    {
        db = json::parse(in); // то есть db принимает json и может обрращаться к нему. и писать подобное db[name]["type"] = "set"; db[name]["data"] = json::array();
    }

    stringstream ss(query);

    string cmd, name;

    ss >> cmd >> name;

    Command command = toCommand(cmd);

    if (command == Command::UNKNOWN)
    {
        cout << "Неизвестная команда\n";
        return 1;
    }

    if (name.empty())
    {
        cout << "Укажите имя структуры\n";
        return 1;
    }

    switch (command)
    {
    case Command::SADD:
    case Command::SREM:
    case Command::SISMEMBER:
        handleSet(command, name, ss, db);
        break;

    case Command::SPUSH:
    case Command::SPOP:
        handleStack(command, name, ss, db);
        break;

    case Command::QPUSH:
    case Command::QPOP:
        handleQueue(command, name, ss, db);
        break;

    case Command::HSET:
    case Command::HDEL:
    case Command::HGET:
        handleHT(command, name, ss, db);
        break;
    }

    ofstream out(fileName);

    if (!out.is_open())
    {
        cout << "Не удалось открыть файл для записи\n";
        return 1;
    }
    out << db.dump(4);
    return 0;
}