#pragma once

#include "ListNode.h"

template <typename T>
class LinkedList;

template <typename T>
class ListNodeIt {
    ListNode<T>* currentNode_;

    friend class LinkedList<T>;

   public:
    ListNodeIt() : currentNode_(nullptr) {}
    ListNodeIt(ListNode<T>* node) : currentNode_(node) {}
    ListNodeIt(const ListNodeIt<T>& other) : currentNode_(other.currentNode_) {}
    ListNodeIt(ListNodeIt<T>&& other);

    ListNodeIt<T>& operator=(const ListNodeIt<T>& other);
    ListNodeIt<T>& operator=(ListNodeIt<T>&& other);

    ListNodeIt<T>& operator++();
    ListNodeIt<T>& operator--();

    ListNodeIt<T> next() const;
    ListNodeIt<T> prev() const;

    bool isValid() const;

    T& operator*();
    const T& operator*() const;

    bool operator==(const ListNodeIt<T>& other) const;
    bool operator!=(const ListNodeIt<T>& other) const;
};

// Constructors
template <typename T>
ListNodeIt<T>::ListNodeIt(ListNodeIt<T>&& other) : currentNode_(other.currentNode_) {
    other.currentNode_ = nullptr;
}

// Assignment operators

template <typename T>
ListNodeIt<T>& ListNodeIt<T>::operator=(const ListNodeIt<T>& other) {
    currentNode_ = other.currentNode_;
    return *this;
}

template <typename T>
ListNodeIt<T>& ListNodeIt<T>::operator=(ListNodeIt<T>&& other) {
    currentNode_ = other.currentNode_;
    other.currentNode_ = nullptr;
    return *this;
}

// Increment / Decrement operators

template <typename T>
ListNodeIt<T>& ListNodeIt<T>::operator++() {
    if (currentNode_ == nullptr)
        throw std::runtime_error("Tried to increment invalid iterator");
    currentNode_ = currentNode_->next.get();
    return *this;
}

template <typename T>
ListNodeIt<T>& ListNodeIt<T>::operator--() {
    if (currentNode_ == nullptr)
        throw std::runtime_error("Tried to decrement invalid iterator");
    currentNode_ = currentNode_->prev;
    return *this;
}

template <typename T>
ListNodeIt<T> ListNodeIt<T>::next() const {
    return currentNode_->next == nullptr ? nullptr : currentNode_->next.get();
}

template <typename T>
ListNodeIt<T> ListNodeIt<T>::prev() const {
    return currentNode_->prev == nullptr ? nullptr : currentNode_->prev.get();
}

template <typename T>
bool ListNodeIt<T>::isValid() const {
    return currentNode_ != nullptr;
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