#ifndef SINGLY_LINKED_LIST_HPP
#define SINGLY_LINKED_LIST_HPP

template<class T>
class Node
{
public:
    explicit Node(const T &mData, Node<T> *next = nullptr);
    inline Node<T>* next() const noexcept { return mNext; }
    inline const T& data() const noexcept { return mData; }
    //inline T& data() noexcept { return mData; }
    inline bool hasNext() const noexcept { return this->mNext; }
    inline void setData(const T &data) { mData = data; }
    void insertAfter(Node<T> *node);
    void removeAfter();
    template<class K, class V>
    friend class HashTable;
private:

    T mData;
    Node<T> *mNext;

};

template<class T>
Node<T>::Node(const T &data, Node<T> *next):
    mData{data}, mNext{next}
{}

template<class T>
void Node<T>::insertAfter(Node<T> *node)
{
    Node<T> *next = this->mNext;
    this->mNext = node;
    node->mNext = next;
}

template<class T>
void Node<T>::removeAfter()
{
    Node<T> *nodeToRemove = this->mNext;
    if(nodeToRemove)
    {
        this->mNext = nodeToRemove->next();
        delete nodeToRemove;
    }
}

template<class T>
class LinkedList
{
public:
    explicit LinkedList();
    LinkedList(const LinkedList<T> &other);
    LinkedList(LinkedList<T> &&other);
    LinkedList<T>& operator=(const LinkedList<T> &rhs);
    LinkedList<T>& operator=(LinkedList<T> &&rhs);
    ~LinkedList();
    inline Node<T>* head() const noexcept { return mHead; }
    inline int count() const noexcept { return mCount; }
    inline bool isEmpty() const noexcept { return mCount == 0; }
    void pushFront(const T &item);
    void insertAt(Node<T> *posToInsert, const T &data);
    void pushBack(const T &data);
    void popFront();
    void removeAt(Node<T>* posToRemove);
    void popBack();
    void clear();
    void updateAt(Node<T> *posToUpdate, const T &data);
    void copyList(const LinkedList<T> &otherList);
    void print();
private:
    Node<T> *mHead {nullptr};
    size_t mCount {0u};
};

template<class T>
LinkedList<T>::LinkedList():
    mCount{0u}
{}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &other)
{
    if(this == &other) return;
    this->clear();
    this->copyList(other);
}

template<class T>
LinkedList<T>::LinkedList(LinkedList<T> &&other):
    mCount{ other.mCount },
    mHead{ std::move(other).mHead }
{}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T> &rhs)
{
    if(this == &rhs) return *this;
    this->clear();
    this->copyList(rhs);
    return *this;
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T> &&rhs)
{
    if(this == &rhs) *this;
    mCount = rhs.mCount;
    mHead = std::move(rhs).mHead;
    return *this;
}


template<class T>
LinkedList<T>::~LinkedList()
{
    clear();
}

template<class T>
void LinkedList<T>::pushFront(const T &item)
{
    Node<T> *node = new Node<T>(item, mHead);
    mHead = node;
    ++mCount;
}

template<class T>
void LinkedList<T>::insertAt(Node<T> *posToInsert, const T &data)
{
    if(!posToInsert) return;
    Node<T> *nextNext = posToInsert->next();
    Node<T> *node = new Node<T>(data, nextNext);
    posToInsert->insertAfter(node);
    ++mCount;
}

template<class T>
void LinkedList<T>::pushBack(const T &data)
{
    if(!mHead)
        this->pushFront(data);
    else
    {
        Node<T> *it = mHead;
        while(it->next())
            it = it->next();
        this->insertAt(it, data);
    }
}

template<class T>
void LinkedList<T>::popFront()
{
    if(!mHead) return;
    Node<T> *oldHead = mHead;
    mHead = mHead->next();
    delete oldHead;
    --mCount;
}

template<class T>
void LinkedList<T>::removeAt(Node<T>* posToRemove)
{
    Node<T> *it = mHead;
    if(!it || !posToRemove) return;
    if(it == posToRemove)
    {
        popFront();
        return;
    }
    while(it->next() != posToRemove)
        it = it->next();
    it->removeAfter();
    --mCount;
}

template<class T>
void LinkedList<T>::popBack()
{
    if(!mHead) return;
    Node<T> *prev = mHead;
    Node<T> *it = prev->next();
    if(!it)
    {
        popFront();
        --mCount;
        return;
    }
    while(it->next())
    {
        prev = it;
        it = it->next();
    }
    prev->removeAfter();
    --mCount;
}

template<class T>
void LinkedList<T>::clear()
{
    while(mHead) this->popFront();
}

template<class T>
void LinkedList<T>::updateAt(Node<T> *posToUpdate, const T &data)
{
    if(!posToUpdate) return;
    posToUpdate->setData(data);
}

template<class T>
void LinkedList<T>::copyList(const LinkedList<T> &otherList)
{
    Node<T> *otherListIterator = otherList.head();
    Node<T> *it = mHead;
    if(it)
    {
        while(it->next()) it = it->next();
        while(otherListIterator)
        {
            Node<T> *node = new Node<T>(otherListIterator->data());
            it->insertAfter(node);
            it = it->next();
            otherListIterator = otherListIterator->next();
            ++mCount;
        }
    }
    else
    {
        while(otherListIterator)
        {
            this->pushBack(otherListIterator->data());
            otherListIterator = otherListIterator->next();
        }
    }
}

template<class T>
void LinkedList<T>::print()
{
    Node<T> *it = mHead;
    std::cout << "\n---------------------\n";
    while (it) {
        std::cout << it->data() << "-";
        it = it->next();
    }
    std::cout << std::endl << "Count = " << mCount << std::endl;
}

#endif // SINGLY_LINKED_LIST_HPP
