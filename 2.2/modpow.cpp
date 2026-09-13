#include "modpow.h"
#include "math_utils.h"

int64_t modpow_fast(int64_t a, int64_t x, int64_t p,
                    std::vector<int64_t>& bits,
                    std::vector<int64_t>& bases,
                    std::vector<int64_t>& results) {
    int64_t res = 1;
    int64_t base = a % p;

    while (x > 0) {
        int64_t bit = x % 2;

        bits.push_back(bit);
        bases.push_back(base);

        if (bit == 1) {
            res = (__int128)res * base % p;
        }
        results.push_back(res);

        base = (__int128)base * base % p;
        x = x / 2;
    }
    return res;
}

int64_t modpow_fermat(int64_t a, int64_t x, int64_t p,
                      std::vector<int64_t>& bits,
                      std::vector<int64_t>& bases,
                      std::vector<int64_t>& results) {
    int64_t x_small = x % (p - 1);
    return modpow_fast(a, x_small, p, bits, bases, results);
}