#pragma once
#include <fstream>
#include <string>

// Общие функции для бинарной сериализации.
// Число пишется как 4 байта, строка - как длина (число) + сами байты.

inline void writeInt(std::ofstream& out, int value)
{
    out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

inline int readInt(std::ifstream& in)
{
    int value = 0;
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

inline void writeString(std::ofstream& out, const std::string& text)
{
    writeInt(out, static_cast<int>(text.size()));
    out.write(text.data(), static_cast<std::streamsize>(text.size()));
}

inline std::string readString(std::ifstream& in)
{
    int len = readInt(in);
    std::string text(len, ' ');
    in.read(text.data(), len);
    return text;
}
