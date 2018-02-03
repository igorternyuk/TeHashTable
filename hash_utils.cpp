#include "hash_utils.hpp"
#include <iostream>

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

size_t hash_string(const char* str, size_t m)
{
    size_t a {7};
    while(getGreatestCommonDivisor(a,m) > 1)
        a = getPrimeNumberGreaterThan(a);
    std::cout << "a= " << a << std::endl;
    size_t hash {0};
    for(; *str != 0; ++str)
        hash = (hash + a + *str) % m;
    return hash;
}

size_t hash_string2(const std::string &keyString, size_t hashSize)
{
    size_t sum {0};
    for(size_t i {0}; i < keyString.size(); ++i)
    {
        sum <<= 3;
        sum += keyString[i];
    }
    return sum % hashSize;
}

size_t hash_sedgwick(const std::string &keyString, size_t hashSize)
{
    size_t hash, i, a = 31415, b = 27183;
    for(i = 0, hash = 0; i < keyString.size(); ++i, a = a * b % (hashSize - 1))
    {
        hash = (a * hash + keyString[i]) % hashSize;
    }
    return hash;
}

size_t getGreatestCommonDivisor (size_t firstNumber, size_t secondNumber)
{
    size_t x;
    while (secondNumber)
    {
        x = firstNumber % secondNumber;
        firstNumber = secondNumber;
        secondNumber = x;
    }
    return firstNumber;
}

bool isPrime(size_t number)
{
    if((number % 2 == 0 && number != 2) || (number % 3 == 0 && number != 3))
        return false;
    for(size_t i = 1; 36 * i * i - 12 * i < number; ++i)
    {
        if(number % (6 * i + 1) == 0 || number % (6 * i - 1) == 0)
            return false;
    }
    return true;
}

bool isPrime2(size_t number){

    if(number == 0) return false;
    if(number ==  1 || number == 2) return true;
    if(number % 2 == 0) return false;
    for(size_t i = 3; i*i <= number; i += 2){
        if(number % i == 0 ) return false;
    }
    return true;

}

size_t getPrimeNumberGreaterThan(size_t number)
{
    if(number % 2 == 0) ++number;
    size_t i = number;
    do
    {
        i += 2;
    } while(!isPrime(i));
    return i;
}
