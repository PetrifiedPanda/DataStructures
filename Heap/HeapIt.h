#pragma once

template <typename T>
class Heap;

template <typename T>
class HeapIt {
    const T* currentItem_;
    size_t currentIndex_;

    friend class Heap<T>;

   public:
    HeapIt() : currentItem_(nullptr), currentIndex_(-1) {}
    HeapIt(const T* ptr, size_t index) : currentItem_(ptr), currentIndex_(index) {}
    HeapIt(const HeapIt<T>& it) : currentItem_(it.currentItem_), currentIndex_(it.currentIndex_) {}

    HeapIt<T>& operator=(const HeapIt<T>& it);

    HeapIt<T>& operator++();
    HeapIt<T>& operator--();

    HeapIt<T>& operator+=(size_t offset);
    HeapIt<T>& operator-=(size_t offset);

    HeapIt<T> operator+(size_t offset) const;
    HeapIt<T> operator-(size_t offset) const;

    size_t operator+(const HeapIt<T>& it) const;
    size_t operator-(const HeapIt<T>& it) const;

    const T& operator[](size_t offset) const;  // Not sure about this one
    const T& operator*() const;

    bool operator==(const HeapIt<T>& it) const;
    bool operator!=(const HeapIt<T>& it) const;
    bool operator<(const HeapIt<T>& it) const;
    bool operator<=(const HeapIt<T>& it) const;
    bool operator>(const HeapIt<T>& it) const;
    bool operator>=(const HeapIt<T>& it) const;
};

template <typename T>
HeapIt<T>& HeapIt<T>::operator=(const HeapIt<T>& it) {
    currentItem_ = it.currentItem_;
    currentIndex_ = it.currentIndex_;
}

// Increment / Decrement operators

template <typename T>
HeapIt<T>& HeapIt<T>::operator++() {
    ++currentItem_;
    ++currentIndex_;
    return *this;
}

template <typename T>
HeapIt<T>& HeapIt<T>::operator--() {
    --currentItem_;
    --currentIndex_;
    return *this;
}

template <typename T>
HeapIt<T>& HeapIt<T>::operator+=(size_t offset) {
    currentItem_ += offset;
    currentIndex_ += offset;
    return *this;
}

template <typename T>
HeapIt<T>& HeapIt<T>::operator-=(size_t offset) {
    currentItem_ -= offset;
    currentIndex_ += offset;
    return *this;
}

// Add / Subtract operators

template <typename T>
HeapIt<T> HeapIt<T>::operator+(size_t offset) const {
    return HeapIt(currentItem_ + offset, currentIndex_ + offset);
}

template <typename T>
HeapIt<T> HeapIt<T>::operator-(size_t offset) const {
    return HeapIt(currentItem_ - offset, currentIndex_ - offset);
}

// Add / Subtract from same type

template <typename T>
size_t HeapIt<T>::operator+(const HeapIt<T>& it) const {
    return currentItem_ + it.currentItem_;
}

template <typename T>
size_t HeapIt<T>::operator-(const HeapIt<T>& it) const {
    return currentItem_ - it.currentItem_;
}

// Dereference operator

template <typename T>
const T& HeapIt<T>::operator[](size_t offset) const {
    return *(currentItem_ + offset);
}

template <typename T>
const T& HeapIt<T>::operator*() const {
    return *currentItem_;
}

// Comparision operators

template <typename T>
bool HeapIt<T>::operator==(const HeapIt<T>& it) const {
    return currentItem_ == it.currentItem_;
}

template <typename T>
bool HeapIt<T>::operator!=(const HeapIt<T>& it) const {
    return !(*this == it);
}

template <typename T>
bool HeapIt<T>::operator<(const HeapIt<T>& it) const {
    return currentItem_ < it.currentItem_;
}

template <typename T>
bool HeapIt<T>::operator<=(const HeapIt<T>& it) const {
    return currentItem_ <= it.currentItem_;
}

template <typename T>
bool HeapIt<T>::operator>(const HeapIt<T>& it) const {
    return currentItem_ < it.currentItem_;
}

template <typename T>
bool HeapIt<T>::operator>=(const HeapIt<T>& it) const {
    return currentItem_ < it.currentItem_;
}

#include "Heap.h"