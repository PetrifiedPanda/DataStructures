#pragma once

#include <memory>

#include "ListNode.h"
#include "ListNodeIt.h"

template <typename T>
class LinkedList {
    std::unique_ptr<ListNode<T>> head_;
    ListNode<T>* tail_;

    friend class ListNodeIt<T>;

   public:
    using iterator = ListNodeIt<T>;

    LinkedList() : head_(nullptr), tail_(nullptr) {}
    LinkedList(const LinkedList<T>& lst);
    LinkedList(LinkedList<T>&& lst) noexcept;
    LinkedList(std::initializer_list<T> lst);

    LinkedList<T>& operator=(const LinkedList<T>& lst);
    LinkedList<T>& operator=(LinkedList<T>&& lst);
    LinkedList<T>& operator=(std::initializer_list<T> lst);

    void append(const T& key);
    void insert(const T& key, iterator position);

    void erase(iterator position);
    void erase(iterator start, iterator end);

    int computeSize() const;
    bool isEmpty() const;
    void clear();

    iterator begin();
    iterator end();
    iterator begin() const;
    iterator end() const;
    iterator cbegin() const;
    iterator cend() const;
};

// Constructors

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& lst) : LinkedList() {
    for (const T& item : lst)
        append(item);
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList<T>&& lst) noexcept : head_(std::move(lst.head_)), tail_(lst.tail_) {
    lst.head_ = nullptr;
    lst.tail_ = nullptr;
}

template <typename T>
LinkedList<T>::LinkedList(std::initializer_list<T> lst) : LinkedList() {
    for (const T& item : lst)
        append(item);
}

// Assignment operators

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& lst) {
    this->clear();

    for (const T& item : lst)
        append(item);

    return *this;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& lst) {
    head_ = std::move(lst.head_);
    tail_ = lst.tail_;
    lst.head_ = nullptr;
    lst.tail_ = nullptr;

    return *this;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(std::initializer_list<T> lst) {
    this->clear();

    for (const T& item : lst)
        append(item);

    return *this;
}

// Insertion functions

template <typename T>
void LinkedList<T>::append(const T& key) {  // O(1)
    if (head_ == nullptr) {
        head_ = std::make_unique<ListNode<T>>(key);
        tail_ = head_.get();
    } else {
        tail_->next = std::make_unique<ListNode<T>>(key);
        tail_->next->prev = tail_;
        tail_ = tail_->next.get();
    }
}

template <typename T>
void LinkedList<T>::insert(const T& key, iterator position) {  // O(1)
    std::unique_ptr<ListNode<T>> newNode = std::make_unique<ListNode<T>>(key);
    if (head_ == nullptr) {
        head_ = std::move(newNode);
        tail_ = head_.get();
    } else if (position == begin()) {
        head_->prev = newNode.get();
        newNode->next = std::move(head_);
        head_ = std::move(newNode);
    } else if (position == end()) {
        newNode->prev = tail_;
        tail_->next = std::move(newNode);
        tail_ = tail_->next.get();
    } else {
        ListNode<T>* prevNode = (--position).currentNode_;
        newNode->next = std::move(prevNode->next);
        newNode->next->prev = newNode.get();
        newNode->prev = prevNode;
        prevNode->next = std::move(newNode);
    }
}

// Deletion functions

template <typename T>
void LinkedList<T>::erase(iterator position) {  // O(1)
    ListNode<T>* toDelete = position.currentNode_;
    if (toDelete == nullptr || head_ == nullptr)
        return;

    if (toDelete == head_.get()) {
        if (head_->next == nullptr) {
            head_ = nullptr;
            tail_ = nullptr;
        } else {
            head_ = std::move(head_->next);
        }
    }

    if (toDelete->next != nullptr)
        toDelete->next->prev = toDelete->prev;

    if (toDelete->prev != nullptr)
        toDelete->prev->next = std::move(toDelete->next);
}

template <typename T>
void LinkedList<T>::erase(iterator start, iterator end) {  // O(1)
    ListNode<T>* startNode = start.currentNode_;
    ListNode<T>* endNode = end.currentNode_;

    if (startNode == nullptr)
        return;

    if (startNode == head_.get()) {
        if (endNode == nullptr) {
            head_ = nullptr;
            tail_ = nullptr;
        } else {
            head_ = std::move(endNode->prev->next);
            head_->prev = nullptr;
        }
    } else {
        ListNode<T>* prevNode = startNode->prev;

        if (endNode == nullptr) {
            prevNode->next = nullptr;
            tail_ = prevNode;
        } else {
            prevNode->next = std::move(endNode->prev->next);
            endNode->prev = prevNode;
        }
    }
}

// Utility functions

template <typename T>
int LinkedList<T>::computeSize() const {  // O(n)
    int result = 0;
    for (const auto& item : *this)
        ++result;

    return result;
}

template <typename T>
bool LinkedList<T>::isEmpty() const {
    return head_ == nullptr;
}

template <typename T>
void LinkedList<T>::clear() {
    head_ = nullptr;
    tail_ = nullptr;
}

// begin / end functions

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::begin() {
    return iterator(head_.get(), *this);
}

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::end() {
    return iterator(nullptr, *this);
}

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::begin() const {
    return cbegin();
}

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::end() const {
    return cend();
}

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::cbegin() const {
    return ListNodeIt<T>(head_.get(), *this);
}

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::cend() const {
    return iterator(nullptr, *this);
}