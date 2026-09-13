#include "shamir.h"
#include "modpow.h"
#include <vector>

int64_t shamir_step(int64_t value, int64_t key, int64_t p) {
    std::vector<int64_t> steps;
    return modpow_fast(value, key, p, steps);
}