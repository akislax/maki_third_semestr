#include "math_utils.h"

bool is_prime(int64_t n) {
    if (n < 2) {
        return false;
    }
    for (int64_t d = 2; d < n; d++) {
        if (n % d == 0) {
            return false;
        }
    }
    return true;
}