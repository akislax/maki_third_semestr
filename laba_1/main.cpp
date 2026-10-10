#include "commands.h"
#include "hand.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        cout << "Использование: ./dbms --file <файл> --query '<команда>'\n";
        return 1;
    }
    string fileName = argv[2];
    string query = argv[4];

    json db = json::object();
    ifstream in(fileName);
    if (in.is_open() && in.peek() != EOF)
    {
        db = json::parse(in);
    }
    in.close();

    stringstream ss(query);
    string cmd, name;
    ss >> cmd >> name;

    Command command = toCommand(cmd);
    if (command == Command::UNKNOWN)
    {
        cout << "неизвестная команда\n";
        return 1;
    }
    if (name == "")
    {
        cout << "укажи имя структуры\n";
        return 1;
    }

    switch (cmd[0])
    {
        case 'M': handleArray(command, db, name, ss); break;
        case 'F': handleFlist(command, db, name, ss); break;
        case 'L': handleDlist(command, db, name, ss); break;
        case 'S': handleStack(command, db, name, ss); break;
        case 'Q': handleQueue(command, db, name, ss); break;
        case 'T': handleTree(command, db, name, ss);  break;
        case 'P': handlePrint(db, name, ss);          break;
    }

    ofstream out(fileName);
    out << db.dump(4) << "\n";
    out.close();

    return 0;
}