#include "shamir.h"
#include "modpow.h"
#include <vector>

int64_t shamir_step(int64_t value, int64_t key, int64_t p) {
    std::vector<int64_t> bits;
    std::vector<int64_t> bases;
    std::vector<int64_t> results;

    return modpow_fast(value, key, p, bits, bases, results);
}