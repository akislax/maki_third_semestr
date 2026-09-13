#include "modpow.h"
#include "math_utils.h"
int64_t modpow_fast(int64_t a, int64_t x, int64_t p,
                    std::vector<int64_t>& steps) {
    int64_t res = 1;
    int64_t base = a % p;

    while (x > 0) {
        if (x % 2 == 1) {
            res = res * base % p;
        }
        steps.push_back(res);

        base = base * base % p;
        x = x / 2;
    }
    return res;
}

int64_t modpow_fermat(int64_t a, int64_t x, int64_t p,
                      std::vector<int64_t>& steps) {
    int64_t x_small = x % (p - 1);
    return modpow_fast(a, x_small, p, steps);
}