#ifndef MODPOW_H
#define MODPOW_H

#include <cstdint>
#include <vector>

int64_t modpow_fast(int64_t a, int64_t x, int64_t p,
                    std::vector<int64_t>& steps);

int64_t modpow_fermat(int64_t a, int64_t x, int64_t p,
                      std::vector<int64_t>& steps);
#endif
