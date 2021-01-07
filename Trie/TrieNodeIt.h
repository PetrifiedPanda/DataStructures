#pragma once

#include "TrieNode.h"

template <typename T>
class Trie;

template <typename T, typename Container>
class TrieNodeIt {
    TrieNode<T>* currentNode_;
    Container sequence_;

   public:
    TrieNodeIt();
    TrieNodeIt(TrieNode<T>* node, const Container& sequence);
    TrieNodeIt(TrieNode<T>* node, Container&& sequence);
    TrieNodeIt(const TrieNodeIt<T, Container>& other);
    TrieNodeIt(TrieNodeIt<T, Container>&& other) noexcept;

    TrieNodeIt<T, Container>& operator=(const TrieNodeIt<T, Container>& other);
    TrieNodeIt<T, Container>& operator=(TrieNodeIt<T, Container>&& other);

    bool operator==(const TrieNodeIt<T, Container>& other) const;
    bool operator!=(const TrieNodeIt<T, Container>& other) const;

    bool isValid() const;

    bool isKey() const;

    const Container& sequence() const;

    void enter(const T& item); // TODO: Find better name
    void enterIfValid(const T& item);

    void removeLast();
};

// Constructors

template <typename T, typename Container>
TrieNodeIt<T, Container>::TrieNodeIt() : currentNode_(nullptr) {}

template <typename T, typename Container>
TrieNodeIt<T, Container>::TrieNodeIt(TrieNode<T>* node, const Container& sequence) : currentNode_(node), sequence_(sequence) {}

template <typename T, typename Container>
TrieNodeIt<T, Container>::TrieNodeIt(TrieNode<T>* node, Container&& sequence) : currentNode_(node), sequence_(std::move(sequence)) {}

template <typename T, typename Container>
TrieNodeIt<T, Container>::TrieNodeIt(const TrieNodeIt<T, Container>& other) : currentNode_(other.currentNode_), sequence_(other.sequence_) {}

template <typename T, typename Container>
TrieNodeIt<T, Container>::TrieNodeIt(TrieNodeIt<T, Container>&& other) noexcept : currentNode_(other.currentNode_), sequence_(std::move(other.sequence_)) {
    other.currentNode_ = nullptr;
}

// Assignment operators

template <typename T, typename Container>
TrieNodeIt<T, Container>& TrieNodeIt<T, Container>::operator=(const TrieNodeIt<T, Container>& other) {
    currentNode_ = other.currentNode_;
    sequence_ = other.sequence_;
}

template <typename T, typename Container>
TrieNodeIt<T, Container>& TrieNodeIt<T, Container>::operator=(TrieNodeIt<T, Container>&& other) {
    currentNode_ = other.currentNode_;
    sequence_ = std::move(other.sequence_);
    other.currentNode_ = nullptr;
}

// Comparision operators

template <typename T, typename Container>
bool TrieNodeIt<T, Container>::operator==(const TrieNodeIt<T, Container>& other) const {
    return currentNode_ == other.currentNode_;
}

template <typename T, typename Container>
bool TrieNodeIt<T, Container>::operator!=(const TrieNodeIt<T, Container>& other) const {
    return currentNode_ == other.currentNode_;
}

template <typename T, typename Container>
bool TrieNodeIt<T, Container>::isValid() const {
    return currentNode_ != nullptr;
}

template <typename T, typename Container>
bool TrieNodeIt<T, Container>::isKey() const {
    return currentNode_->isKey;
}

template <typename T, typename Container>
const Container& TrieNodeIt<T, Container>::sequence() const {
    return sequence_;
}

template <typename T, typename Container>
void TrieNodeIt<T, Container>::enter(const T& item) {
    if (currentNode_->children.count(item) > 0) {
        currentNode_ = currentNode_->children[item];
        sequence_.push_back(item);
    } else
        throw std::runtime_error("Tried to traverse to nonexistant child");
}

template <typename T, typename Container>
void TrieNodeIt<T, Container>::enterIfValid(const T& item) {
    if (currentNode_->children.count(item) > 0) {
        currentNode_ = currentNode_->children[item];
        sequence_.push_back(item);
    }
}

template <typename T, typename Container>
void TrieNodeIt<T, Container>::removeLast() {
    currentNode_ = currentNode_->parent;
    sequence_.resize(sequence_.size() - 1);
}

#include "Trie.h"