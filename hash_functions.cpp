#include "hash_functions.hpp"

#define HASH32_S 2654435769

constexpr double GOLDEN_RATIO { (sqrt(5) - 1) / 2 };

size_t hash1(int key, size_t max)
{
    return key % max;
}

size_t hash32(int key, size_t max)
{
    auto p = int(ceil(log(max) / log(2)));
    return (key * HASH32_S) >> (32 - p);
    //h(k) = (k * s mod 2^32) >> (32 - p)
}
