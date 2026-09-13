#include "euclid.h"

int64_t ext_gcd(int64_t a, int64_t b, int64_t& u, int64_t& v) {
    if (b == 0) {
        u = 1;
        v = 0;
        return a;
    }

    int64_t u1, v1;
    int64_t g = ext_gcd(b, a % b, u1, v1);

    u = v1;
    v = u1 - (a / b) * v1;

    return g;
}

int64_t ext_gcd_table(int64_t a, int64_t b,
                      std::vector<int64_t>& qs,
                      std::vector<int64_t>& rs,
                      std::vector<int64_t>& us,
                      std::vector<int64_t>& vs,
                      int64_t& u, int64_t& v) {
    int64_t r0 = a, r1 = b;
    int64_t u0 = 1, u1 = 0;
    int64_t v0 = 0, v1 = 1;

    while (r1 != 0) {
        int64_t q = r0 / r1;

        int64_t r2 = r0 - q * r1;
        int64_t u2 = u0 - q * u1;
        int64_t v2 = v0 - q * v1;

        qs.push_back(q);
        rs.push_back(r2);
        us.push_back(u2);
        vs.push_back(v2);

        r0 = r1;
        r1 = r2;
        u0 = u1;
        u1 = u2;
        v0 = v1;
        v1 = v2;
    }

    u = u0;
    v = v0;
    return r0;
}

int64_t mod_inverse(int64_t c, int64_t m, bool& exists) {
    int64_t u, v;
    int64_t g = ext_gcd(c, m, u, v);

    if (g != 1) {
        exists = false;
        return 0;
    }

    exists = true;
    return ((u % m) + m) % m;
}

int64_t mod_inverse_table(int64_t c, int64_t m, bool& exists,
                          std::vector<int64_t>& qs,
                          std::vector<int64_t>& rs,
                          std::vector<int64_t>& us,
                          std::vector<int64_t>& vs,
                          int64_t& u_raw) {
    int64_t u, v;
    int64_t g = ext_gcd_table(c, m, qs, rs, us, vs, u, v);

    u_raw = u;

    if (g != 1) {
        exists = false;
        return 0;
    }

    exists = true;
    return ((u % m) + m) % m;
}