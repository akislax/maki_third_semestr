#include "euclid.h"

int64_t ext_gcd(int64_t a, int64_t b, int64_t& x, int64_t& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    int64_t x1, y1;
    int64_t g = ext_gcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return g;
}

int64_t mod_inverse(int64_t c, int64_t m, bool& exists) {
    int64_t x, y;
    int64_t g = ext_gcd(c, m, x, y);

    if (g != 1) {
        exists = false;
        return 0;
    }

    exists = true;
    return ((x % m) + m) % m;
}