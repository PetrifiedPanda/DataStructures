#pragma once

#include <cmath>
#include <functional>
#include <vector>

template <typename T>
class Heap {
    std::vector<T> data_;
    std::function<bool(const T&, const T&)> comparator_;

   public:
    explicit Heap(const std::function<bool(const T&, const T&)>& comp) : comparator_(comp) {}
    Heap(std::initializer_list<T> init, const std::function<bool(const T&, const T&)>& comp);

    Heap(Heap<T>& heap) : data_(heap.data_), comparator_(heap.comparator_) {}
    Heap(Heap<T>&& heap) noexcept : data_(std::move(heap.data_)), comparator_(std::move(heap.comparator_)) {}

    Heap<T>& operator=(const Heap<T>& heap);
    Heap<T>& operator=(Heap<T>&& heap);
    Heap<T>& operator=(std::initializer_list<T> init);

    void setComparator(const std::function<bool(const T&, const T&)>& comp);

    void insert(const T& key);

    T extractExtremum();

    void clear();

    size_t size();

   private:
    void downHeapify(size_t startIndex);
    void biDirHeapify(size_t startIndex);

    void buildHeap();

    size_t left(size_t index);
    size_t right(size_t index);
    size_t parent(size_t index);

    void swap(size_t i1, size_t i2);
};

// Constructors

template <typename T>
Heap<T>::Heap(std::initializer_list<T> init, const std::function<bool(const T&, const T&)>& comp) : Heap(comp) {
    data_ = init;
    buildHeap();
}

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
    data_ = init;
    buildHeap();
    return *this;
}

template <typename T>
void Heap<T>::setComparator(const std::function<bool(const T&, const T&)>& comp) {
    comparator_ = comp;
    buildHeap();
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
void Heap<T>::clear() {
    data_.clear();
}

template <typename T>
size_t Heap<T>::size() {
    return data_.size();
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

        if (parent(prevIndex) >= 0 && parent(prevIndex) < data_.size() && comparator_(data_[maxIndex], data_[parent(prevIndex)]))
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
void Heap<T>::buildHeap() {
    int size = data_.size();
    for (int i = ceil((size - 1) / 2.0) - 1; i > -1; --i)
        downHeapify(i);
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