#include "file.h"
#include <fstream>

std::vector<int64_t> read_file_bytes(const std::string& name) {
    std::vector<int64_t> data;
    std::ifstream f(name, std::ios::binary);
    char c;
    while (f.get(c)) {
        data.push_back((unsigned char)c);
    }
    return data;
}

void write_numbers(const std::string& name, const std::vector<int64_t>& data) {
    std::ofstream f(name);
    for (size_t i = 0; i < data.size(); i++) {
        f << data[i] << " ";
    }
}

std::vector<int64_t> read_numbers(const std::string& name) {
    std::vector<int64_t> data;
    std::ifstream f(name);
    int64_t v;
    while (f >> v) {
        data.push_back(v);
    }
    return data;
}

void write_file_bytes(const std::string& name, const std::vector<int64_t>& data) {
    std::ofstream f(name, std::ios::binary);
    for (size_t i = 0; i < data.size(); i++) {
        f.put((char)data[i]);
    }
}