#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include "array_list.hpp"
#include "singly_linked_list.hpp"

inline bool isPrime(size_t number)
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

inline bool isPrime2(size_t number){

    if(number == 0) return false;
    if(number ==  1 || number == 2) return true;
    if(number % 2 == 0) return false;
    for(size_t i = 3; i*i <= number; i += 2){
        if(number % i == 0 ) return false;
    }
    return true;

}

inline size_t getPrimeNumberGreaterThan(size_t number)
{
    if(number % 2 == 0) ++number;
    size_t i = number;
    for(; !isPrime(i); i += 2);
    return i;
}

template<class K, class V>
class Map
{
public:
    explicit Map() = default;
    Map(const Map<K,V> &other) = default;
    Map(Map<K,V> &&other) = default;
    Map<K,V>& operator=(const Map<K,V> &rhs) = default;
    Map<K,V>& operator=(Map<K,V> &&rhs) = default;
    virtual ~Map() {}
    inline auto count() const noexcept { return mCount; }
    inline auto isEmpty() const noexcept { return mCount == 0; }
    virtual void insert(const K &key, const V &value) = 0;
    virtual void update(const K &key, const V &value) = 0;
    virtual void remove(const K &key) = 0;
    virtual bool find(const K &key, V &value) const = 0;
    virtual const V get(const K &key) const = 0;
protected:
    size_t mCount {0u};
};

template<class K, class V>
struct Pair
{
    K key;
    V value;
};

template<class K, class V>
class HashTable: public Map<K,V>
{
public:
    explicit HashTable(size_t bucketsNumber,
                       std::function<size_t(const K &key, size_t max)> hf);
    HashTable(const HashTable<K,V> &other) = default;
    HashTable(HashTable<K,V> &&other) = default;
    HashTable<K,V>& operator=(const HashTable<K,V> &rhs) = default;
    HashTable<K,V>& operator=(HashTable<K,V> &&rhs) = default;
    virtual ~HashTable() = default;
    virtual void insert(const K &key, const V &value) override;
    virtual void update(const K &key, const V &value) override;
    virtual void remove(const K &key) override;
    virtual bool find(const K &key, V &value) const override;
    virtual const V get(const K &key) const override;
    void clear();
    const V operator[](const K &key) const;
    V& operator[](const K &key);
    virtual void print() const;

private:

    Array<LinkedList<Pair<K,V>>> mBuckets;
    std::function<size_t(const K &key, size_t max)> mHashFunction;
    size_t mCount {0u};
    auto findPosition(const K &key) const;
    template<class Key, class Value>
    friend class HashTableIterator;
};

template<class K, class V>
HashTable<K,V>::HashTable(size_t bucketsNumber,
                          std::function<size_t(const K &, size_t max)> hf):
    mBuckets(getPrimeNumberGreaterThan(bucketsNumber)), mHashFunction(hf)
{
    for(size_t i{0u}; i < mBuckets.capacity(); ++i)
        mBuckets.add(LinkedList<Pair<K,V>>());
}

template<class K, class V>
void HashTable<K,V>::insert(const K &key, const V &value)
{
    auto hash = mHashFunction(key, mBuckets.capacity());
    LinkedList<Pair<K,V>> &bucket = mBuckets[hash];
    Pair<K,V> pair = {key, value};

    if(bucket.isEmpty())
    {
        bucket.pushFront(pair);
        ++mCount;
    }
    else
    {
        auto it = bucket.head();
        auto prev = it;
        if(key <= it->data().key)
        {
            if(key == it->data().key)
            {
                bucket.updateAt(it, pair);
            }
            else
            {
                bucket.pushFront(pair);
                ++mCount;
            }
        }
        else
        {
            while(it && key > it->data().key)
            {
                prev = it;
                it = it->next();
            }

            if(it && it != prev && prev->data().key == it->data().key) //If the list already have item with such key
                bucket.updateAt(it, pair);         //we will update corresponding value
            else
            {
                bucket.insertAt(prev, pair);       //otherwise we will insert the new key-value pair
                ++mCount;
            }
        }
    }
}

template<class K, class V>
auto HashTable<K,V>::findPosition(const K &key) const
{
    auto hash = mHashFunction(key, mBuckets.capacity());
    const LinkedList<Pair<K,V>> &bucket = mBuckets[hash];
    auto it = bucket.head();
    while(it && key > it->data().key)
        it = it->next();
    return it;
}

template<class K, class V>
bool HashTable<K,V>::find(const K &key, V &value) const
{
    auto it = findPosition(key);
    if(it && it->data().key == key)
    {
        value = it->data().value;
        return true;
    }
    return false;
}

template<class K, class V>
void HashTable<K,V>::update(const K &key, const V &value)
{
    auto hash = mHashFunction(key, mBuckets.capacity());
    LinkedList<Pair<K,V>> &bucket = mBuckets[hash];
    auto it = bucket.head();
    while(it && key > it->data().key)
        it = it->next();
    //auto it = findPosition(key);
    if(it && it->data().key == key)
    {
        Pair<K,V> pair = {key, value};
        bucket.updateAt(it, pair);
    }
}

template<class K, class V>
void HashTable<K,V>::remove(const K &key)
{
    auto hash = mHashFunction(key, mBuckets.capacity());
    LinkedList<Pair<K,V>> &bucket = mBuckets[hash];
    auto it = bucket.head();
    while(it && key > it->data().key)
        it = it->next();
    //auto it = findPosition(key);
    if(it && it->data().key == key)
    {
        bucket.removeAt(it);
        --mCount;
    }
}

//const K &key
template<class K, class V>
const V HashTable<K,V>::get(const K &key) const
{
    V val;
    if(!find(key, val))
        val = findPosition(key)->mData.value;
    return val;
}

template<class K, class V>
const V HashTable<K,V>::operator[](const K &key) const
{
    return get(key);
}

template<class K, class V>
V& HashTable<K,V>::operator[](const K &key)
{
    V val;
    if(!find(key, val)) this->insert(key, val);
    return findPosition(key)->mData.value;
}

template<class K, class V>
void HashTable<K,V>::clear()
{
   for(size_t i{0u}; i < mBuckets.size(); ++i)
       mBuckets[i].clear();
   mCount = 0;
}

template<class K, class V>
void HashTable<K,V>::print() const
{
    for(size_t i {0u}; i < mBuckets.size(); ++i)
    {
        auto lst = mBuckets[i];
        for(int i = 0; i < 20; ++i)
            std::cout << "---";
        std::cout << std::endl;
        std::cout << "| " << i << " | ";
        if(!lst.isEmpty())
        {
            for(auto it = lst.head(); it != nullptr; it = it->next())
                std::cout << " (" << it->data().key << "," << it->data().value << ") ->";
        }
        std::cout << std::endl;
        for(int i = 0; i < 20; ++i)
            std::cout << "---";
        std::cout << std::endl;
    }
}

template<class K, class V>
class HashTableIterator
{
public:
    explicit HashTableIterator(HashTable<K,V> &ht);
    virtual void reset();
    virtual void next();
    virtual void setValue(const V &value);
    virtual const Pair<K, V>& getData() const noexcept;
    inline bool end() const noexcept { return mIsEndOfTable; }
    inline void setHashTable(HashTable<K,V> &ht) { mHashTable = &ht; }
private:
    HashTable<K,V> *mHashTable;
    size_t mCurrentBucket {0u};
    Node<Pair<K,V>> *mCurrentPosition { nullptr };
    bool mIsEndOfTable { false };
    void searchNextAvailableNode(size_t startIndex);
};

template<class K, class V>
HashTableIterator<K,V>::HashTableIterator(HashTable<K,V> &ht):
    mHashTable(&ht)
{
    searchNextAvailableNode(0);
}

template<class K, class V>
void HashTableIterator<K,V>::reset()
{
    searchNextAvailableNode(0);
    mIsEndOfTable = false;
}

template<class K, class V>
void HashTableIterator<K,V>::next()
{
    mCurrentPosition = mCurrentPosition->next();
    if(!mCurrentPosition)
        searchNextAvailableNode(++mCurrentBucket);
}

template<class K, class V>
void HashTableIterator<K,V>::setValue(const V &value)
{
    Pair<K,V> pair = {mCurrentPosition->data().key, value};
    mCurrentPosition->setData(pair);
}

template<class K, class V>
const Pair<K, V>& HashTableIterator<K,V>::getData() const noexcept
{
    return mCurrentPosition->data();
}

template<class K, class V>
void HashTableIterator<K,V>::searchNextAvailableNode(size_t startIndex)
{
    mIsEndOfTable = true;
    auto hashTableSize = mHashTable->mBuckets.size();
    if(startIndex >= hashTableSize) return;
    for(size_t i { startIndex }; i < hashTableSize; ++i)
    {
        if(!mHashTable->mBuckets[i].isEmpty())
        {
            mCurrentBucket = i;
            mCurrentPosition = mHashTable->mBuckets[mCurrentBucket].head();
            mIsEndOfTable = false;
            return;
        }
    }
}


//Open adressing

enum class HashTableItemStatus
{
    EMPTY,
    OCUPIED,
    DELETED
};

template<class K, class V>
struct HashTableItem
{

};

template<class K, class V>
class OpenAddressingHashTable
{
public:
    explicit OpenAddressingHashTable(size_t size);
private:

};

#endif // HASHTABLE_HPP
