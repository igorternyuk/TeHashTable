#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP

#include <cstdlib>

template<class T>
class Array
{
public:
    explicit Array(size_t capacity = 31u);
    Array(const Array<T> &other);
    Array(Array<T> &&other);
    Array<T>& operator=(const Array<T> &other);
    Array<T>& operator=(Array<T> &&other);
    ~Array();
    inline size_t size() const noexcept { return mSize; }
    inline size_t capacity() const noexcept { return mCapacity; }
    inline float loadFactor() const noexcept { return static_cast<float>(mSize) / mCapacity; }
    inline bool isEmpty() const noexcept { return mSize == 0; }
    void add(const T &item);
    void removeAt(size_t index);
    int search(const T& elementToSearch) const noexcept;
    T fetchMax() const noexcept;
    T fetchMin() const noexcept;
    T& getElementAt(size_t index);
    void resize(size_t newCapacity);
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    operator T* (void) const;
private:
    size_t mSize, mCapacity;
    T *mData;
    void ensureCapacity();
};

template<class T>
Array<T>::Array(size_t capacity):
    mSize{0u}, mCapacity{capacity}
{
    mData = new T[capacity];
}

template<class T>
Array<T>::Array(const Array<T> &other):
    mSize{other.mSize},
    mCapacity{other.mCapacity}

{
    mData = new T[mSize];
    for(size_t i{0u}; i < mSize; ++i)
        mData[i] = other.mData[i];
}

template<class T>
Array<T>::Array(Array<T> &&other):
    mSize{other.mSize},
    mCapacity{other.mCapacity},
    mData{std::move(other).mData}
{
    other.mSize = 0;
    other.mCapacity = 0;
    other.mData = nullptr;
}

template<class T>
Array<T>& Array<T>::operator=(const Array<T> &other)
{
    if(this == &other) return *this;
    mSize = other.mSize;
    mCapacity = other.mCapacity;
    delete [] mData;
    mData = new T[mSize];
    for(size_t i{0u}; i < mSize; ++i)
        mData[i] = other.mData[i];
    return *this;
}

template<class T>
Array<T>& Array<T>::operator=(Array<T> &&other)
{
    mSize = other.mSize;
    mCapacity = other.mCapacity;
    delete [] mData;
    mData = std::move(other).mData;
    other.mSize = 0;
    other.mCapacity = 0;
    other.mData = nullptr;
    return *this;
}

template<class T>
Array<T>::~Array()
{
    delete [] mData;
}

template<class T>
int Array<T>::search(const T& elementToSearch) const noexcept
{
    for(size_t i{0u}; i < mSize; ++i)
        if(mData[i] == elementToSearch)
            return i;
    return -1;
}

template<class T>
T Array<T>::fetchMax() const noexcept
{
    T maxElement = mData[0];
    for(size_t i{0u}; i < mSize; ++i)
        if(mData[i] > maxElement)
            maxElement = mData[i];
    return maxElement;
}

template<class T>
T Array<T>::fetchMin() const noexcept
{
    T minElement = mData[0];
    for(size_t i{0u}; i < mSize; ++i)
        if(mData[i] < minElement)
            minElement = mData[i];
    return minElement;
}

template<class T>
void Array<T>::add(const T &item)
{
    if(mSize >= mCapacity)
        ensureCapacity();
    mData[mSize++] = item;
}

template<class T>
void Array<T>::removeAt(size_t index)
{
    if(index >= this->mSize) return;
    for(size_t i = index; i < mSize - 1; ++i)
        mData[i] = mData[i + 1];
    --mSize;
}

template<class T>
void Array<T>::resize(size_t newCapacity)
{
    if(newCapacity == mCapacity) return;

    T* newData = new T[newCapacity];

    auto numOfElementsToSave = mSize < newCapacity ? mSize : newCapacity;

    size_t i = 0u;
    for(; i < numOfElementsToSave ; ++i)
        newData[i] = mData[i];
    mSize = i;
    mCapacity = newCapacity;
    delete [] mData;
    mData = newData;
}

template<class T>
T& Array<T>::getElementAt(size_t index)
{
    return mData[index < mSize ? index : mSize - 1];
}

template<class T>
T& Array<T>::operator[](size_t index)
{
    return mData[index < mSize ? index : mSize - 1];
}

template<class T>
const T& Array<T>::operator[](size_t index) const
{
    return mData[index < mSize ? index : mSize - 1];
}

template <class T>
Array<T>::operator T* (void) const
{
    return mData;
}

template<class T>
void Array<T>::ensureCapacity()
{
    T* newData = new T[2 * mCapacity];
    for(size_t i {0u}; i < mSize; ++i)
        newData[i] = mData[i];
    mCapacity *= 2;
    delete [] mData;
    mData = newData;
    std::cout << "Array capacity was increased to " << mCapacity << std::endl;
}

#endif // ARRAY_LIST_HPP
