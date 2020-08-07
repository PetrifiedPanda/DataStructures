#pragma once

#include "LinkedList.h"

template <typename T>
class SizeLinkedList : public LinkedList<T> {
    int size_;

   public:
    using iterator = ListNodeIt<T>;

    SizeLinkedList() : LinkedList<T>(), size_(0) {}
    SizeLinkedList(const LinkedList<T>& list);
    SizeLinkedList(const SizeLinkedList<T>& list);
    SizeLinkedList(LinkedList<T>&& list) noexcept;
    SizeLinkedList(SizeLinkedList<T>&& list) noexcept;
    SizeLinkedList(std::initializer_list<T> list);

    SizeLinkedList<T>& operator=(const SizeLinkedList<T>& list);
    SizeLinkedList<T>& operator=(const LinkedList<T>& list);
    SizeLinkedList<T>& operator=(SizeLinkedList<T>&& list);
    SizeLinkedList<T>& operator=(LinkedList<T>&& list);
    SizeLinkedList<T>& operator=(std::initializer_list<T> list);

    void append(const T& key);
    void insert(const T& key, iterator position);

    void erase(iterator position);
    void erase(iterator start, iterator end);

    void clear();

    int size() const;

   private:
    using LinkedList<T>::computeSize;
};

// Constructor

template <typename T>
SizeLinkedList<T>::SizeLinkedList(const LinkedList<T>& list) : LinkedList<T>(list), size_(list.computeSize()) {}

template <typename T>
SizeLinkedList<T>::SizeLinkedList(const SizeLinkedList<T>& list) : LinkedList<T>(list), size_(list.size()) {}

template <typename T>
SizeLinkedList<T>::SizeLinkedList(LinkedList<T>&& list) noexcept : LinkedList<T>(std::move(list)), size_(LinkedList<T>::computeSize()) {}

template <typename T>
SizeLinkedList<T>::SizeLinkedList(SizeLinkedList<T>&& list) noexcept : LinkedList<T>(std::move(list)), size_(list.size()) {}

template <typename T>
SizeLinkedList<T>::SizeLinkedList(std::initializer_list<T> list) {
    for (const auto& item : list)
        append(item);
}

// Assignment operators

template <typename T>
SizeLinkedList<T>& SizeLinkedList<T>::operator=(const SizeLinkedList<T>& list) {
    LinkedList<T>::operator=(list);
    size_ = list.size_;
}

template <typename T>
SizeLinkedList<T>& SizeLinkedList<T>::operator=(const LinkedList<T>& list) {
    LinkedList<T>::operator=(list);
    size_ = LinkedList<T>::computeSize();
}

template <typename T>
SizeLinkedList<T>& SizeLinkedList<T>::operator=(SizeLinkedList<T>&& list) {
    LinkedList<T>::operator=(list);
    size_ = list.size_;
}

template <typename T>
SizeLinkedList<T>& SizeLinkedList<T>::operator=(LinkedList<T>&& list) {
    LinkedList<T>::operator=(list);
    size_ = LinkedList<T>::computeSize();
}

template <typename T>
SizeLinkedList<T>& SizeLinkedList<T>::operator=(std::initializer_list<T> list) {
    for (const auto& item : list)
        append(item);
}

// Insertion functions

template <typename T>
void SizeLinkedList<T>::append(const T& key) {
    LinkedList<T>::append(key);
    ++size_;
}

template <typename T>
void SizeLinkedList<T>::insert(const T& key, iterator position) {
    LinkedList<T>::insert(key, position);
    ++size_;
}

// Deletion functions

template <typename T>
void SizeLinkedList<T>::erase(iterator position) {
    LinkedList<T>::erase(position);
    --size_;
}

template <typename T>
void SizeLinkedList<T>::erase(iterator start, iterator end) {
    iterator it = start;
    iterator itPrev;
    while (it != end + 1) {
        itPrev = it;
        ++it;
        erase(itPrev);
    }
}

template <typename T>
void SizeLinkedList<T>::clear() {
    LinkedList<T>::clear();
    size_ = 0;
}

template <typename T>
int SizeLinkedList<T>::size() const {
    return size_;
}