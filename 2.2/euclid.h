#ifndef EUCLID_H
#define EUCLID_H

#include <cstdint>

int64_t ext_gcd(int64_t a, int64_t b, int64_t& x, int64_t& y);
int64_t mod_inverse(int64_t c, int64_t m, bool& exists);
#endif