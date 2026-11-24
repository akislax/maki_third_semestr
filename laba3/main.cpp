#include "arrayClass.h"
#include "flistClass.h"
#include "dlistClass.h"
#include "stackClass.h"
#include "queueClass.h"
#include "treeClass.h"
#include "hashTable.h"
#include <fstream>
#include <string>
#include <iostream>

void handlePrint(json& db, const string& name, stringstream& ss)
{
    if (!db.contains(name))
    {
        cout << "нет структуры " << name << "\n";
        return;
    }

    if (db[name]["type"] == "array") handleArray(Command::MPRINT, db, name, ss);
    else if (db[name]["type"] == "flist") handleFlist(Command::FPRINT, db, name, ss);
    else if (db[name]["type"] == "dlist") handleDlist(Command::LPRINT, db, name, ss);
    else if (db[name]["type"] == "stack") handleStack(Command::SPRINT, db, name, ss);
    else if (db[name]["type"] == "queue") handleQueue(Command::QPRINT, db, name, ss);
    else if (db[name]["type"] == "tree") handleTree(Command::TPRINT, db, name, ss);
    else if (db[name]["type"] == "hash") handleHash(Command::HPRINT, db, name, ss);
    else if (db[name]["type"] == "kukushka") handleKukushka(Command::KPRINT, db, name, ss);
}

int main(int argc, char* argv[])
{
    try
    {
    if (argc != 5 && argc != 7)
    {
        cout << "Использование: ./laba3 --file <файл> [--format json|binary] --query '<команда>'\n";
        return 1;
    }

    if (string(argv[1]) != "--file")
    {
        cout << "неверные параметры\n";
        return 1;
    }

    string fileName = argv[2];
    string query;
    if (argc == 5)
    {
        if (string(argv[3]) != "--query")
        {
            cout << "неверные параметры\n";
            return 1;
        }
        query = argv[4];
        binaryMode = false;
    }
    else
    {
        if (string(argv[3]) != "--format" ||
            (string(argv[4]) != "json" && string(argv[4]) != "binary") ||
            string(argv[5]) != "--query")
        {
            cout << "неверные параметры\n";
            return 1;
        }
        binaryMode = (string(argv[4]) == "binary");
        query = argv[6];
    }

    binaryBase = fileName;
    string dbFile = binaryMode ? fileName + ".meta.json" : fileName;

    json db = json::object();
    ifstream in(dbFile);
    if (in.is_open() && in.peek() != EOF)
    {
        db = json::parse(in);
    }
    in.close();

    stringstream ss(query);
    string cmd;
    string name;
    ss >> cmd >> name;

    Command command = toCommand(cmd);
    if (command == Command::UNKNOWN)
    {
        cout << "неизвестная команда\n";
        return 1;
    }
    if (name.empty())
    {
        cout << "укажи имя структуры\n";
        return 1;
    }

    switch (cmd[0])
    {
        case 'M': handleArray(command, db, name, ss);    break;
        case 'F': handleFlist(command, db, name, ss);    break;
        case 'L': handleDlist(command, db, name, ss);    break;
        case 'S': handleStack(command, db, name, ss);    break;
        case 'Q': handleQueue(command, db, name, ss);    break;
        case 'T': handleTree(command, db, name, ss);     break;
        case 'H': handleHash(command, db, name, ss);     break;
        case 'K': handleKukushka(command, db, name, ss); break;
        case 'P': handlePrint(db, name, ss);             break;
        default: cout << "неизвестная команда\n"; break;
    }

    ofstream out(dbFile);
    out << db.dump(4) << "\n";
    out.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}