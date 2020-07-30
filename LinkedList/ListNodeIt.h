#pragma once

#include "ListNode.h"

template <typename T>
class LinkedList;

template <typename T>
class ListNodeIt {
    ListNode<T>* currentNode_;
    const LinkedList<T>& list_;

    friend class LinkedList<T>;

   public:
    ListNodeIt(ListNode<T>* node, const LinkedList<T>& list) : currentNode_(node), list_(list) {}

    ListNodeIt<T>& operator++();
    ListNodeIt<T>& operator--();

    ListNodeIt<T> operator+(int offset);
    ListNodeIt<T> operator-(int offset);

    T& operator*();
    const T& operator*() const;

    bool operator==(const ListNodeIt<T>& other) const;
    bool operator!=(const ListNodeIt<T>& other) const;
};

// Increment / Decrement operators

template <typename T>
ListNodeIt<T>& ListNodeIt<T>::operator++() {  // Not bounds checked
    currentNode_ = currentNode_->next.get();
    return *this;
}

template <typename T>
ListNodeIt<T>& ListNodeIt<T>::operator--() {
    if (currentNode_ == nullptr)
        currentNode_ = list_.tail_;
    else
        currentNode_ = currentNode_->prev;
    return *this;
}

// Addition / Subtraction operators

template <typename T>
ListNodeIt<T> ListNodeIt<T>::operator+(int offset) {  // These are sadly not O(1)
    ListNodeIt<T> result(currentNode_, list_);

    for (int i = 0; i < offset; ++i)
        ++result;

    return result;
}

template <typename T>
ListNodeIt<T> ListNodeIt<T>::operator-(int offset) {  // These are sadly not O(1)
    ListNodeIt<T> result(currentNode_, list_);

    for (int i = 0; i < offset; ++i)
        --result;

    return result;
}

// Dereference operators

template <typename T>
T& ListNodeIt<T>::operator*() {
    return currentNode_->key;
}

template <typename T>
const T& ListNodeIt<T>::operator*() const {
    return currentNode_->key;
}

// Comparision operators

template <typename T>
bool ListNodeIt<T>::operator==(const ListNodeIt<T>& other) const {
    return currentNode_ == other.currentNode_;
}

template <typename T>
bool ListNodeIt<T>::operator!=(const ListNodeIt<T>& other) const {
    return !(*this == other);
}

#include "LinkedList.h"