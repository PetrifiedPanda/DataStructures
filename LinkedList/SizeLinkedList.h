#pragma once

#include "LinkedList.h"

template <typename T>
class SizeLinkedList : public LinkedList<T> {
    int size_;

   public:
    using iterator = ListNodeIt<T>;

    SizeLinkedList() : LinkedList<T>(), size_(0) {}
    SizeLinkedList(LinkedList<T>& list);
    SizeLinkedList(SizeLinkedList<T>& list);
    SizeLinkedList(LinkedList<T>&& list);
    SizeLinkedList(SizeLinkedList<T>&& list);
    SizeLinkedList(std::initializer_list<T> list);

    void append(T key);
    void insert(T key, iterator position);

    void erase(iterator position);
    void erase(iterator start, iterator end);

    void clear();

    int size() const;
};

template <typename T>
SizeLinkedList<T>::SizeLinkedList(LinkedList<T>& list) : LinkedList<T>(list), size_(list.computeSize()) {}

template <typename T>
SizeLinkedList<T>::SizeLinkedList(SizeLinkedList<T>& list) : LinkedList<T>(list), size_(list.size()) {}

template <typename T>
SizeLinkedList<T>::SizeLinkedList(LinkedList<T>&& list) : LinkedList<T>(std::move(list)), size_(LinkedList<T>::computeSize()) {}

template <typename T>
SizeLinkedList<T>::SizeLinkedList(SizeLinkedList<T>&& list) : LinkedList<T>(std::move(list)), size_(list.size()) {}

template <typename T>
SizeLinkedList<T>::SizeLinkedList(std::initializer_list<T> list) {
    for (const auto& item : list)
        append(item);
}

template <typename T>
void SizeLinkedList<T>::append(T key) {
    LinkedList<T>::append(key);
    ++size_;
}

template <typename T>
void SizeLinkedList<T>::insert(T key, iterator position) {
    LinkedList<T>::insert(key, position);
    ++size_;
}

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