#include "arrayClass.h"

void Array::resizeArray()
{
    int newCapacity = capacity * 2;
    string* newData = new string[newCapacity];
    for (int i = 0; i < size; i++)
    {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

Array::Array()
{
    capacity = 4;
    data = new string[capacity];
    size = 0;
}

Array::~Array()
{
    delete[] data;
}

void Array::pushBack(const string& text)
{
    if (size >= capacity)
    {
        resizeArray();
    }
    data[size] = text;
    size++;
}

void Array::insertAt(const string& text, int index)
{
    if (index < 0 || index > size)
    {
        cout << "такого индекса нету\n";
        return;
    }

    if (size == capacity)
    {
        resizeArray();
    }

    // Первым пересаживается последний: справа от него свободно.
    // Если начать с начала - затрём соседа справа раньше, чем он успеет пересесть.
    //
    // Пример: a b c d, вставляем X на место 1
    //   i = 3: data[4] = data[3]  ->  a b c d d
    //   i = 2: data[3] = data[2]  ->  a b c c d
    //   i = 1: data[2] = data[1]  ->  a b b c d
    //   i = 0: 0 >= 1 неверно, цикл закончился
    for (int i = size - 1; i >= index; i--)
    {
        data[i + 1] = data[i];
    }

    data[index] = text;
    size++;
}

void Array::removeAt(int index)
{
    if (index < 0 || index >= size)
    {
        cout << "такого индекса нету\n";
        return;
    }

    // Пример: a b c d, удаляем элемент 1 (b)
    //   i = 1: data[1] = data[2]  ->  a c c d
    //   i = 2: data[2] = data[3]  ->  a c d d
    // size-- -> последняя d больше не считается
    for (int i = index; i < size - 1; i++)
    {
        data[i] = data[i + 1];
    }
    size--;
}

void Array::setAt(const string& text, int index)
{
    if (index < 0 || index >= size)
    {
        cout << "такого индекса нету\n";
        return;
    }
    data[index] = text;
}

string Array::getAt(int index) const
{
    if (index < 0 || index >= size)
    {
        cout << "Нет такого индекса\n";
        return "";
    }
    return data[index];
}

void Array::printArray() const
{
    for (int i = 0; i < size; i++)
    {
        cout << data[i] << " ";
    }
    cout << "(size=" << size << ", capacity=" << capacity << ")\n";
}

int Array::lenght() const
{
    return size;
}

void handleArray(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "array")) return;

    Array arr;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            arr.pushBack(element.get<string>());
        }
    }

    int index;
    string value;
    switch (command)
    {
        case Command::MPUSH:
            ss >> value;
            arr.pushBack(value);
            cout << "-> " << value << "\n";
            break;
        case Command::MINSERT:
            ss >> index >> value;
            arr.insertAt(value, index);
            break;
        case Command::MGET:
            ss >> index;
            cout << "-> " << arr.getAt(index) << "\n";
            break;
        case Command::MSET:
            ss >> index >> value;
            arr.setAt(value, index);
            break;
        case Command::MDEL:
            ss >> index;
            arr.removeAt(index);
            break;
        case Command::MLEN:
            cout << "-> " << arr.lenght() << "\n";
            break;
        case Command::MPRINT:
            arr.printArray();
            break;
        default:
            break;
    }

    json data = json::array();
    for (int i = 0; i < arr.lenght(); i++)
    {
        data.push_back(arr.getAt(i));
    }
    db[name]["type"] = "array";
    db[name]["data"] = data;
}

// сериализация: [количество] [строка] [строка] ...
void Array::saveBinary(const string& fileName) const
{
    ofstream out(fileName, ios::binary);
    writeInt(out, size);
    for (int i = 0; i < size; i++)
    {
        writeString(out, data[i]);
    }
}

void Array::loadBinary(const string& fileName)
{
    ifstream in(fileName, ios::binary);
    if (!in.is_open()) return;

    size = 0;                                  // очищаем массив перед загрузкой
    int count = readInt(in);
    for (int i = 0; i < count; i++)
    {
        pushBack(readString(in));
    }
}
