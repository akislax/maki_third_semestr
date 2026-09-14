#ifndef IO_H
#define IO_H

#include <cstdint>
#include <vector>
#include <string>

// таблица
void print_table(const std::vector<std::string>& headers,
                 const std::vector<std::vector<int64_t> >& columns,
                 const std::vector<std::string>& notes);

void print_menu();

//2 и 3
void print_euclid_table(const std::vector<int64_t>& qs,
                        const std::vector<int64_t>& rs,
                        const std::vector<int64_t>& us,
                        const std::vector<int64_t>& vs);

//1
void print_fermat_check(int64_t a, int64_t x, int64_t p);
void print_simple_steps(const std::vector<int64_t>& steps, int64_t answer);
void print_binary_steps(int64_t x,
                        const std::vector<int64_t>& bits,
                        const std::vector<int64_t>& bases,
                        const std::vector<int64_t>& results,
                        int64_t answer);
void print_compare(int64_t answer1, int64_t answer2);

//2
void print_gcd_result(int64_t a, int64_t b, int64_t g, int64_t u, int64_t v);

//3
void print_inverse_result(int64_t c, int64_t m, bool exists,
                          int64_t d, int64_t u_raw);

//4 и 5
void print_keys(int64_t p, int64_t cA, int64_t cB, int64_t dA, int64_t dB);
void print_shamir_steps(int64_t p, int64_t m, int64_t cA, int64_t cB,
                        int64_t dA, int64_t dB,
                        int64_t x1, int64_t x2, int64_t x3, int64_t x4);
void print_byte_step(size_t index, int64_t byte,
                     int64_t x1, int64_t x2, int64_t x3);

#endif