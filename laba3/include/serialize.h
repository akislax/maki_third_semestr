#pragma once
#include <cstdint>
#include <fstream>
#include <string>

// Число записывается как 4 байта в little-endian.
// Строка: длина (4 байта), затем сами байты.
inline void writeInt(std::ofstream& out, int value)
{
    uint32_t number = static_cast<uint32_t>(value);
    for (int i = 0; i < 4; i++)
    {
        out.put(static_cast<char>((number >> (i * 8)) & 0xFF));
    }
}

inline int readInt(std::ifstream& in)
{
    uint32_t number = 0;
    for (int i = 0; i < 4; i++)
    {
        int byte = in.get();
        if (byte == EOF) return 0;
        number |= static_cast<uint32_t>(static_cast<unsigned char>(byte)) << (i * 8);
    }
    return static_cast<int32_t>(number);
}

inline void writeString(std::ofstream& out, const std::string& text)
{
    writeInt(out, static_cast<int>(text.size()));
    out.write(text.data(), static_cast<std::streamsize>(text.size()));
}

inline std::string readString(std::ifstream& in)
{
    int len = readInt(in);
    if (len < 0) return "";
    std::string text(static_cast<size_t>(len), '\0');
    in.read(text.data(), static_cast<std::streamsize>(len));
    return text;
}
