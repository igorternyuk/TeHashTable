#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include "array_list.hpp"
#include "singly_linked_list.hpp"
#include "hash_utils.hpp"



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
    int test {0};
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
protected:
    using Map<K,V>::mCount;
private:

    Array<LinkedList<Pair<K,V>>> mBuckets;
    std::function<size_t(const K &key, size_t max)> mHashFunction;
    auto findPosition(const K &key) const;
    template<class Key, class Value>
    friend class HashTableIterator;
};

template<class K, class V>
HashTable<K,V>::HashTable(size_t bucketsNumber,
                          std::function<size_t(const K &, size_t max)> hf):
    Map<K,V>::Map(),mBuckets(getPrimeNumberGreaterThan(bucketsNumber)), mHashFunction(hf)
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
    if(find(key, val))
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
    K key;
    V value;
    HashTableItemStatus status;
};

enum class CollisionResolutionMethod
{
    LINEAR_PROBING,
    QUADRATIC_PROBING,
    DOUBLE_HASHING
};

template<class K, class V>
class OpenAddressingHashTable : public Map<K,V>
{
public:
    explicit OpenAddressingHashTable(size_t tableSize,
                                     std::function<size_t(const K &key, size_t maxVal)> hf,
                                     CollisionResolutionMethod probingType,
                                     std::function<size_t(const K &key, size_t maxVal)> hf2);
    OpenAddressingHashTable(const OpenAddressingHashTable<K,V> &other) = default;
    OpenAddressingHashTable(OpenAddressingHashTable<K,V> &&other) = default;
    OpenAddressingHashTable<K,V>& operator=(const OpenAddressingHashTable<K,V> &rhs) = default;
    OpenAddressingHashTable<K,V>& operator=(OpenAddressingHashTable<K,V> &&rhs) = default;
    //virtual ~OpenAddressingHashTable() {}
    // Map interface
    virtual void insert(const K &key, const V &value);
    virtual void update(const K &key, const V &value);
    virtual void remove(const K &key);
    virtual bool find(const K &key, V &value) const;
    virtual const V get(const K &key) const;
    virtual void print() const noexcept;
    V& operator[](const K &key);
    const V operator[](const K &key) const;
    void clear();
//private:
    Array<HashTableItem<K,V>> mData;
    std::function<size_t(const K &key, size_t maxVal)> mHashFunction;
    CollisionResolutionMethod mProbingType;
    std::function<size_t(const K &key, size_t maxVal)> mHashFunction2;
    size_t mNumberOfOcupied;
    inline double getFillFactor() const noexcept { return double(mNumberOfOcupied) / mData.size(); }
protected:
    using Map<K,V>::mCount;
private:
    void insertIntoArray(Array<HashTableItem<K,V>> &targetArray, const K &key,
                         const V &value);
    bool has(const K &key, size_t &pos) const;
};

template<class K, class V>
OpenAddressingHashTable<K,V>::OpenAddressingHashTable(size_t tableSize,
                                                      std::function<size_t(const K &, size_t maxSize)> hf,
                                                      CollisionResolutionMethod probingType,
                                                      std::function<size_t(const K &key, size_t maxVal)> hf2):
    Map<K,V>::Map(),mData(getPrimeNumberGreaterThan(2 * tableSize)), mHashFunction{hf},
    mProbingType{probingType}, mHashFunction2{hf2}
{
    //HashTableItem<K,V> item = {K(),V(),HashTableItemStatus::EMPTY};
    for(size_t i{0u}; i < mData.capacity(); ++i)
        mData.add(HashTableItem<K,V>());

}

template<class K, class V>
void OpenAddressingHashTable<K,V>::insert(const K &key, const V &value)
{
    insertIntoArray(mData, key, value);
    ++mNumberOfOcupied;
    if(getFillFactor() > 0.7f)
    {
        std::cout << "Hash table is 70% full" << std::endl;
        Array<HashTableItem<K,V>> newData{2 * mData.capacity()};
        for(size_t i{0u}; i < newData.capacity(); ++i)
            newData.add(HashTableItem<K,V>());
        for(size_t i{0u}; i < mData.size(); ++i)
        {
            if(mData[i].status == HashTableItemStatus::OCUPIED)
            {
                insertIntoArray(newData, mData[i].key, mData[i].value);
            }
        }
        mData = newData;
    }
}

template<class K, class V>
bool OpenAddressingHashTable<K,V>::find(const K &key, V &value) const
{
    size_t pos{0};
    if(has(key, pos))
    {
        value = mData[pos].value;
        return true;
    }
    return false;
}

template<class K, class V>
void OpenAddressingHashTable<K,V>::update(const K &key, const V &value)
{
    size_t pos{0};
    if(has(key, pos))
        mData[pos].value = value;
}

template<class K, class V>
void OpenAddressingHashTable<K,V>::remove(const K &key)
{
    size_t pos{0};
    if(has(key, pos))
    {
        mData[pos].status = HashTableItemStatus::DELETED;
        --mNumberOfOcupied;
    }
}

template<class K, class V>
const V OpenAddressingHashTable<K, V>::get(const K &key) const
{
    V v;
    size_t pos{0};
    auto isFound = has(key, pos);
    return isFound ? mData[pos].value: v;
}

template<class K, class V>
void OpenAddressingHashTable<K,V>::print() const noexcept
{
    for(size_t i{0u}; i < mData.capacity(); ++i)
    {
        std::cout << "| " << i <<  " | =(" << mData[i].key << "," << mData[i].value << ",";
       // auto status = mData[i].status;
        switch(auto status = mData[i].status)
        {
        case HashTableItemStatus::OCUPIED:
            std::cout << "Occupied";
            break;
        case HashTableItemStatus::EMPTY:
            std::cout << "Empty";
            break;
        case HashTableItemStatus::DELETED:
            std::cout << "Deleted";
            break;
        }
        std::cout << ") |" << std::endl;
    }
    std::cout << std::endl;
}

template<class K, class V>
void OpenAddressingHashTable<K,V>::insertIntoArray(
        Array<HashTableItem<K,V>> &targetArray, const K &key,const V &value)
{
    auto targetIndex = mHashFunction(key, targetArray.size());
    size_t numOfProbe {0u};
    while(targetArray[targetIndex].status == HashTableItemStatus::OCUPIED)
    {
        std::cout << std::endl << "Collision detected for index = " << targetIndex << std::endl;
        size_t step{1};
        switch(mProbingType)
        {
        case CollisionResolutionMethod::QUADRATIC_PROBING:
            //h(k,i) = (h(k) + c1 * i + c2 * i * i) % m;
            step = numOfProbe * numOfProbe / 2;
            break;
        case CollisionResolutionMethod::DOUBLE_HASHING:
            //h(k,i) = (h1(k) + i * h2(k)) % m
            // h2(k) and m are relatively primes
            step = mHashFunction2(key, targetArray.size()) * numOfProbe;
            break;
        case CollisionResolutionMethod::LINEAR_PROBING:
        default:
            break;
        }
        targetIndex += step;
        targetIndex %= targetArray.size();
        ++numOfProbe;
    }
    targetArray[targetIndex] = {key, value, HashTableItemStatus::OCUPIED};
}

template<class K, class V>
bool OpenAddressingHashTable<K, V>::has(const K &key, size_t &pos) const
{
    auto targetIndex = mHashFunction(key, mData.size());
    size_t numOfProbe {0u};
    while(true)
    {
        size_t step{1};
        switch(mProbingType)
        {
        case CollisionResolutionMethod::QUADRATIC_PROBING:
            //h(k,i) = (h(k) + c1 * i + c2 * i * i) % m;
            step = numOfProbe * numOfProbe / 2;
            break;
        case CollisionResolutionMethod::DOUBLE_HASHING:
            //h(k,i) = (h1(k) + i * h2(k)) % m
            // h2(k) and m are relatively primes
            step = mHashFunction2(key, mData.size()) * numOfProbe;
            break;
        case CollisionResolutionMethod::LINEAR_PROBING:
        default:
            break;
        }
        targetIndex += step;
        targetIndex %= mData.size();
        ++numOfProbe;
        if(mData[targetIndex].status == HashTableItemStatus::EMPTY)
            return false;
        if(mData[targetIndex].status == HashTableItemStatus::OCUPIED &&
           mData[targetIndex].key == key)
        {
            pos = targetIndex;
            return true;
        }
    }
    return false;
}
template<class K, class V>
V& OpenAddressingHashTable<K, V>::operator[](const K &key)
{
    size_t pos{0};
    if(!has(key, pos)) this->insert(key, V());
    has(key, pos);
    return mData[pos].value;
}

template<class K, class V>
const V OpenAddressingHashTable<K, V>::operator[](const K &key) const
{
    return get(key);
}

template<class K, class V>
void OpenAddressingHashTable<K, V>::clear()
{
    for(size_t i{0u}; i < mData.size(); ++i)
        mData[i].status = HashTableItemStatus::EMPTY;
}


#endif // HASHTABLE_HPP
