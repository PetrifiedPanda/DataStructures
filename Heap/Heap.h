#pragma once

#include <cmath>
#include <functional>
#include <vector>

#include "HeapIt.h"

template <typename T>
class Heap {
    std::vector<T> data_;
    std::function<bool(const T&, const T&)> comparator_;

   public:
    using iterator = HeapIt<T>;
    explicit Heap(const std::function<bool(const T&, const T&)>& comp) : comparator_(comp) {}
    Heap(std::initializer_list<T> init, const std::function<bool(const T&, const T&)>& comp);

    Heap(Heap<T>& heap);
    Heap(Heap<T>&& heap) noexcept;

    Heap<T>& operator=(const Heap<T>& heap);
    Heap<T>& operator=(Heap<T>&& heap);

    Heap<T>& operator=(std::initializer_list<T> init);

    void setComparator(const std::function<bool(const T&, const T&)>& comp);

    void insert(const T& key);

    T extractExtremum();

    void erase(iterator it);

    void clear();

    size_t size();

    iterator begin();
    iterator end();
    iterator begin() const;
    iterator end() const;
    iterator cbegin() const;
    iterator cend() const;

   private:
    void downHeapify(size_t startIndex);
    void biDirHeapify(size_t startIndex);

    size_t left(size_t index);
    size_t right(size_t index);
    size_t parent(size_t index);

    void swap(size_t i1, size_t i2);
};

// Constructors

template <typename T>
Heap<T>::Heap(std::initializer_list<T> init, const std::function<bool(const T&, const T&)>& comp) : Heap(comp) {
    data_.reserve(init.size());
    for (const auto& item : init)
        data_.push_back(item);

    int size = init.size();
    for (int i = ceil((size - 1) / 2) - 1; i > -1; --i)
        downHeapify(i);
}

template <typename T>
Heap<T>::Heap(Heap<T>& heap) : data_(heap.data_), comparator_(heap.comparator_) {}

template <typename T>
Heap<T>::Heap(Heap<T>&& heap) noexcept : data_(std::move(heap.data_)), comparator_(std::move(heap.comparator_)) {}

// Equality operators

template <typename T>
Heap<T>& Heap<T>::operator=(const Heap<T>& heap) {
    data_ = heap.data_;
    comparator_ = heap.comparator_;
    return *this;
}

template <typename T>
Heap<T>& Heap<T>::operator=(Heap<T>&& heap) {
    data_ = std::move(heap.data_);
    comparator_ = std::move(heap.comparator_);
    return *this;
}

template <typename T>
Heap<T>& Heap<T>::operator=(std::initializer_list<T> init) {
    data_.clear();
    data_.reserve(init.size());
    for (const auto& item : init)
        data_.push_back(item);

    int size = init.size();
    for (int i = ceil((size - 1) / 2) - 1; i > -1; --i)
        downHeapify(i);
}

template <typename T>
void Heap<T>::setComparator(const std::function<bool(const T&, const T&)>& comp) {
    comparator_ = comp;

    int size = data_.size();
    for (int i = ceil((size - 1) / 2) - 1; i > -1; --i)
        downHeapify(i);
}

// Insert operation

template <typename T>
void Heap<T>::insert(const T& key) {
    data_.push_back(key);

    size_t i = data_.size() - 1;
    while (i > 0 && comparator_(data_[i], data_[parent(i)])) {
        swap(i, parent(i));
        i = parent(i);
    }
}

// Delete operations

template <typename T>
T Heap<T>::extractExtremum() {
    T max = data_[0];
    data_[0] = data_[data_.size() - 1];
    data_.erase(--data_.end());
    downHeapify(0);
    return max;
}

template <typename T>
void Heap<T>::erase(iterator it) {
    size_t deleteIndex = it.currentIndex_;
    data_[deleteIndex] = data_[data_.size() - 1];
    data_.erase(--data_.end());
    if (deleteIndex != data_.size())
        biDirHeapify(deleteIndex);
}

template <typename T>
void Heap<T>::clear() {
    data_.clear();
}

template <typename T>
size_t Heap<T>::size() {
    return data_.size();
}

// Iterators

template <typename T>
typename Heap<T>::iterator Heap<T>::begin() {
    return iterator(&data_[0], 0);
}

template <typename T>
typename Heap<T>::iterator Heap<T>::end() {
    return iterator(&data_[0] + data_.size(), data_.size());
}

template <typename T>
typename Heap<T>::iterator Heap<T>::begin() const {
    return cbegin();
}

template <typename T>
typename Heap<T>::iterator Heap<T>::end() const {
    return cend();
}

template <typename T>
typename Heap<T>::iterator Heap<T>::cbegin() const {
    return iterator(&data_[0], 0);
}

template <typename T>
typename Heap<T>::iterator Heap<T>::cend() const {
    return iterator(&data_[0] + data_.size(), data_.size());
}

// Utility functions

template <typename T>
void Heap<T>::downHeapify(size_t startIndex) {
    int maxIndex = startIndex;
    int prevIndex = -1;
    while (maxIndex != prevIndex) {
        prevIndex = maxIndex;

        if (left(prevIndex) < data_.size() && !comparator_(data_[maxIndex], data_[left(prevIndex)]))
            maxIndex = left(prevIndex);

        if (right(prevIndex) < data_.size() && !comparator_(data_[maxIndex], data_[right(prevIndex)]))
            maxIndex = right(prevIndex);

        if (maxIndex != prevIndex)
            swap(prevIndex, maxIndex);
    }
}

template <typename T>
void Heap<T>::biDirHeapify(size_t startIndex) {
    int maxIndex = startIndex;
    int prevIndex = -1;
    while (maxIndex != prevIndex) {
        prevIndex = maxIndex;

        if (parent(prevIndex) >= 0 && parent(prevIndex) < data_.size() && !comparator_(data_[parent(prevIndex)], data_[maxIndex]))
            maxIndex = parent(prevIndex);
        else {
            if (left(prevIndex) < data_.size() && !comparator_(data_[maxIndex], data_[left(prevIndex)]))
                maxIndex = left(prevIndex);

            if (right(prevIndex) < data_.size() && !comparator_(data_[maxIndex], data_[right(prevIndex)]))
                maxIndex = right(prevIndex);
        }

        if (maxIndex != prevIndex)
            swap(prevIndex, maxIndex);
    }
}

template <typename T>
size_t Heap<T>::left(size_t index) {
    return 2 * (index + 1) - 1;
}

template <typename T>
size_t Heap<T>::right(size_t index) {
    return 2 * (index + 1);
}

template <typename T>
size_t Heap<T>::parent(size_t index) {
    return ceil(index / 2.0) - 1;
}

template <typename T>
void Heap<T>::swap(size_t i1, size_t i2) {
    T tmp = std::move(data_[i1]);
    data_[i1] = std::move(data_[i2]);
    data_[i2] = std::move(tmp);
}