#ifndef EUCLID_H
#define EUCLID_H

#include <cstdint>
#include <vector>

int64_t ext_gcd(int64_t a, int64_t b, int64_t& u, int64_t& v);

int64_t ext_gcd_table(int64_t a, int64_t b, std::vector<int64_t>& qs, std::vector<int64_t>& rs, std::vector<int64_t>& us, std::vector<int64_t>& vs, int64_t& u, int64_t& v);

int64_t mod_inverse(int64_t c, int64_t m, bool& exists);

int64_t mod_inverse_table(int64_t c, int64_t m, bool& exists, std::vector<int64_t>& qs, std::vector<int64_t>& rs, std::vector<int64_t>& us, std::vector<int64_t>& vs, int64_t& u_raw);

#endif