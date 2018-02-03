#ifndef HASH_UTILS_HPP
#define HASH_UTILS_HPP

#include <cstdlib>
#include <string>
#include <cmath>

size_t hash1(int key, size_t max);

size_t hash32(int key, size_t max);

size_t hash_string(const char* str, size_t m);

size_t hash_string2(const std::string &keyString, size_t hashSize);

size_t hash_sedgwick(const std::string &keyString, size_t hashSize);

size_t getGreatestCommonDivisor (size_t firstNumber, size_t secondNumber); //Euclid algorithm

bool isPrime(size_t number);

bool isPrime2(size_t number);

size_t getPrimeNumberGreaterThan(size_t number);

#endif // HASH_UTILS_HPP
