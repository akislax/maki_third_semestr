#ifndef FILE_H
#define FILE_H

#include <cstdint>
#include <vector>
#include <string>

std::vector<int64_t> read_file_bytes(const std::string& name);
void write_numbers(const std::string& name, const std::vector<int64_t>& data);
std::vector<int64_t> read_numbers(const std::string& name);
void write_file_bytes(const std::string& name, const std::vector<int64_t>& data);

#endif