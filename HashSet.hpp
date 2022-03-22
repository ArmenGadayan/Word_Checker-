// HashSet.hpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"


template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8, in which case
    // the new capacity should be determined by this formula:
    //
    //     capacity * 2 + 1
    //
    // In the case where the array is resized, this function runs in linear
    // time (with respect to the number of elements, assuming a good hash
    // function); otherwise, it runs in constant time (again, assuming a good
    // hash function).  The amortized running time is also constant.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns false.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;

private:
    HashFunction hashFunction;
    
    int numElements;
    int capacity = DEFAULT_CAPACITY;

    struct Node
    {
        ElementType element;
        Node* next;
    };
    Node** table;

    void resize();
    void resizeAdd(const ElementType& element, int capacity, Node** tempTable);

    // You'll no doubt want to add member variables and "helper" member
    // functions here.
};


namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}

template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
    numElements = 0;
    table = new Node*[DEFAULT_CAPACITY]();
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    for (int i=0; i<capacity; i++)
    {
        Node* cur = table[i];
        while (table[i] != nullptr)
        {
            table[i] = table[i]->next;
            delete cur;
            cur = table[i];
        }
    }
    delete [] table;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{s.hashFunction}
{
    capacity = s.capacity;
    numElements = s.numElements;
    table = new Node*[s.capacity];
    for (int i=0; i<s.capacity; i++)
    {
        if (s.table[i] == nullptr)
            table[i] = nullptr;
        else
        {
            Node* otherCur = s.table[i]->next;
            Node* temp = new Node {s.table[i]->element, otherCur};
            table[i] = temp;
            while (otherCur != nullptr)
            {
                temp->next = new Node {otherCur->element, otherCur->next};
                temp = temp->next;
                otherCur = otherCur->next;
            }
        }
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{s.hashFunction}
{
    numElements = std::exchange(s.numElements, 0);
    capacity = std::exchange(s.capacity, 0);
    table = std::exchange(s.table, nullptr); 
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{    
    for (int i=0; i<capacity; i++)
    {
        Node* cur = table[i];
        while (table[i] != nullptr)
        {
            table[i] = table[i]->next;
            delete cur;
            cur = table[i];
        }
    }
    delete [] table;

    capacity = s.capacity;
    numElements = s.numElements;
    table = new Node*[s.capacity];
    for (int i=0; i<s.capacity; i++)
    {
        if (s.table[i] == nullptr)
            table[i] = nullptr;
        else
        {
            Node* otherCur = s.table[i]->next;
            Node* temp = new Node {s.table[i]->element, otherCur};
            table[i] = temp;
            while (otherCur != nullptr)
            {
                temp->next = new Node {otherCur->element, otherCur->next};
                temp = temp->next;
                otherCur = otherCur->next;
            }
        }
    }
    
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    numElements = std::exchange(s.numElements, 0);
    capacity = std::exchange(s.capacity, capacity);
    table = std::exchange(s.table, table);
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::resizeAdd(const ElementType& element, int cap, Node** tempTable)
{
    unsigned int hashValue = hashFunction(element)%cap;

    Node* newNode = new Node{element, nullptr};

    if (tempTable[hashValue] == nullptr)
        tempTable[hashValue] = newNode;
    else 
    {
        Node* cur = tempTable[hashValue];
        while (cur->next != nullptr)
        {
            cur = cur->next;
        }
        cur->next = newNode;
    }
}


template <typename ElementType>
void HashSet<ElementType>::resize()
{
    int oldCapacity = capacity;
    capacity = capacity*2 + 1;
    Node** tempTable = new Node*[capacity]();

    for (int i=0; i < oldCapacity; i++)
    {
        if (table[i] != nullptr)
        {
            resizeAdd(table[i]->element, capacity, tempTable);
            Node* cur = table[i]->next;
            delete table[i];
            Node* nodeToDelete;

            while (cur != nullptr)
            {
                resizeAdd(cur->element, capacity, tempTable);
                nodeToDelete = cur;
                cur = cur->next;
                delete nodeToDelete;
            }
        }
    }
    delete [] table;

    table = tempTable;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    if (!contains(element))
    {
        Node* newNode = new Node{element, nullptr};
        unsigned int hashValue = hashFunction(element)%capacity;
        
        if (table[hashValue] == nullptr)
            table[hashValue] = newNode;
        else 
        {
            Node* cur = table[hashValue];
            while (cur->next != nullptr)
            {
                cur = cur->next;
            }
            cur->next = newNode;
        }
        numElements += 1;
        double ratio = 1.0*numElements/capacity;
        if (ratio > 0.8)
            resize();
    }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int hashValue = hashFunction(element)%capacity;

    if (table[hashValue] != nullptr)
    {
        Node* cur = table[hashValue];
        while (cur != nullptr)
        {
            if (cur->element == element)
                return true;
                
            cur = cur->next;
        }
        return false;
    }
    else
        return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return numElements;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    if (index >= capacity)
        return 0;

    int count = 0;
    Node* cur = table[index];
    while (cur != nullptr)
    {
        count += 1;
        cur = cur->next;
    }
    return count;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    if (index >= capacity)
        return false;

    Node* cur = table[index];
    while (cur != nullptr)
    {
        if (cur->element == element)
            return true;
        cur = cur->next;
    }
    return false;
}


#endif

