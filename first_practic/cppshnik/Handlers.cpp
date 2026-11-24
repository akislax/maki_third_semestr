#include "Handlers.h"
#include "Stack.h"
#include "Set.h"
#include "HashTable.h"
#include "Queue.h"
#include <iostream>

void handleSet(Command command, const std::string &name, std::stringstream &ss, json &db)
{
    if (!db.contains(name))
    {
        db[name]["type"] = "set";
        db[name]["data"] = json::array();
    }
    if (db[name]["type"] != "set")
    {
        std::cout << "Структура имеет другой тип\n";
        return;
    }
    Set mySet;

    for (const auto &item : db[name]["data"])
    {
        mySet.add(item.get<std::string>());
    }
    std::string value;
    if (command == Command::SADD)
    {
        ss >> value;
        if (value == "")
        {
            std::cout << "Нельзя добавить пустой элемент\n";
            return;
        }
        else
        {
            if (mySet.search(value))
            {
                std::cout << "Элемент уже существует" << '\n';
            }
            else
            {
                mySet.add(value);
                std::cout << value << '\n';
            }
        }
    }

    if (command == Command::SREM)
    {
        ss >> value;
        if (value == "")
        {
            std::cout << "Нельзя удалить пустой элемент\n";
            return;
        }
        else
        {
            if (mySet.remove(value))
            {
                std::cout << value << '\n';
            }
            else
            {
                std::cout << "Элемент не найден" << "\n";
            }
        }
    }

    if (command == Command::SISMEMBER)
    {
        ss >> value;
        if (value == "")
        {
            std::cout << "Нельзя найти пустой элемент\n";
            return;
        }
        if (mySet.search(value))
        {
            std::cout << "TRUE\n";
        }
        else
        {
            std::cout << "FALSE\n";
        }
    }

    json data = json::array();

    for (size_t i = 0; i < mySet.getCapacity(); i++)
    {
        for (NodeSet *current = mySet.getTable()[i]; current != nullptr; current = current->next)
        {
            data.push_back(current->value);
        }
    }

    db[name]["data"] = data;
}

void handleStack(Command command, const std::string &name, std::stringstream &ss, json &db)
{
    if (!db.contains(name))
    {
        db[name]["type"] = "stack";
        db[name]["data"] = json::array();
    }
    if (db[name]["type"] != "stack")
    {
        std::cout << "Структура имеет другой тип\n";
        return;
    }
    Stack myStack;

    for (const auto &item : db[name]["data"])
    {
        myStack.push(item.get<std::string>());
    }
    std::string value;
    if (command == Command::SPUSH)
    {
        ss >> value;
        if (value == "")
        {
            std::cout << "Нельзя добавить пустой элемент\n";
            return;
        }
        else
        {
            myStack.push(value);
            std::cout << value << "\n";
        }
    }

    if (command == Command::SPOP)
    {
        if (myStack.isEmptyStack())
        {
            std::cout << "Стек пуст\n";
        }
        else
        {
            std::string valueForSpop = myStack.pop();
            std::cout << valueForSpop << "\n";
        }
    }

    json data = json::array();

    for (SNode *current = myStack.getHead(); current != nullptr; current = current->next)
    {
        data.push_back(current->a);
    }

    db[name]["data"] = data;
}

void handleQueue(Command command, const std::string &name, std::stringstream &ss, json &db)

{
    if (!db.contains(name))
    {
        db[name]["type"] = "queue";
        db[name]["data"] = json::array();
    }
    if (db[name]["type"] != "queue")
    {
        std::cout << "Структура имеет другой тип\n";
        return;
    }
    Queue myQueue;

    for (const auto &item : db[name]["data"])
    {
        myQueue.push(item.get<std::string>());
    }
    std::string value;
    if (command == Command::QPUSH)
    {
        ss >> value;
        if (value == "")
        {
            std::cout << "Нельзя добавить пустой элемент\n";
            return;
        }
        else
        {
            myQueue.push(value);
            std::cout << value << "\n";
        }
    }

    if (command == Command::QPOP)
    {
        if (myQueue.isEmptyQ())
        {
            std::cout << "Очередь пустая\n";
        }
        else
        {
            std::string valueForQpop = myQueue.pop();
            std::cout << valueForQpop << "\n";
        }
    }

    json data = json::array();

    for (QNode *current = myQueue.getHead(); current != nullptr; current = current->next)
    {
        data.push_back(current->a);
    }

    db[name]["data"] = data;
}
void handleHT(Command command, const std::string &name,
              std::stringstream &ss, json &db)
{
    if (!db.contains(name))
    {
        db[name]["type"] = "hashtable";
        db[name]["data"] = json::object();
    }

    if (db[name]["type"] != "hashtable")
    {
        std::cout << "Структура имеет другой тип\n";
        return;
    }

    HashTable myHT;

    for (auto it = db[name]["data"].begin();
         it != db[name]["data"].end(); ++it)
    {
        myHT.add(it.key(), it.value().get<std::string>());
    }

    std::string key, value;

    if (command == Command::HSET)
    {
        ss >> key >> value;

        if (key.empty() || value.empty())
        {
            std::cout << "Укажите ключ и значение\n";
            return;
        }
        else
        {
            myHT.add(key, value);
            std::cout << key << " " << value << "\n";
        }
    }

    if (command == Command::HDEL)
    {
        ss >> key;

        if (key.empty())
        {
            std::cout << "Укажите ключ\n";
            return;
        }
        else
        {
            if (myHT.search(key))
            {
                value = myHT.get(key);
                myHT.remove(key);
                std::cout << key << " " << value << '\n';
            }
            else
            {
                std::cout << "Ключ не найден" << '\n';
            }
        }
    }

    if (command == Command::HGET)
    {
        ss >> key;

        if (key.empty())
        {
            std::cout << "Укажите ключ\n";
            return;
        }

        if (myHT.search(key))
        {
            std::cout << myHT.get(key) << '\n';
        }
        else
        {
            std::cout << "Ключ не найден\n";
        }
    }

    json data = json::object();

    for (size_t i = 0; i < myHT.getCapacity(); i++)
    {
        for (Node *current = myHT.getTable()[i];
             current != nullptr;
             current = current->next)
        {
            data[current->key] = current->value;
        }
    }

    db[name]["data"] = data;
}
