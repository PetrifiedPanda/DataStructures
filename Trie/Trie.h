#pragma once

#include "TrieNodeIt.h"

template <typename T>
class Trie {
    TrieNode<T>* root_;

   public:
    Trie();
    Trie(const Trie& other);
    Trie(Trie&& other) noexcept;
    ~Trie();

    Trie<T>& operator=(const Trie<T>& other);
    Trie<T>& operator=(Trie<T>&& other);

    template <typename Container>
    void insert(const Container& sequence);

    template <typename Container>
    void erase(const Container& sequence);
    
    template <typename Container>
    bool contains(const Container& sequence) const;

    template <typename Container>
    bool containsKey(const Container& sequence) const;

    template <typename Container>
    TrieNodeIt<T, Container> find(const Container& sequence) const;

    template <typename Container>
    TrieNodeIt<T, Container> findKey(const Container& sequence) const;

    template <typename Container, typename ContContainer>
    ContContainer keysWithPrefix(const Container& prefix) const;

   private:
    void free();
    void copy(const Trie<T>& other);
    void move(Trie<T>&& other) noexcept;

    void freeSubtree(TrieNode<T>* subtreeRoot);
    TrieNode<T>* copySubtree(const TrieNode<T>* subtreeRoot);

    template <typename Container>
    TrieNode<T>* findNode(const Container& sequence) const;

    template <typename Container>
    bool eraseRec(TrieNode<T>* currentNode, const Container& sequence, int seqIndexs);

    template <typename Container, typename ContContainer>
    void collect(TrieNode<T>* node, Container& prefix, ContContainer& results) const;
};

// Constructors

template <typename T>
Trie<T>::Trie() : root_(nullptr) {}

template <typename T>
Trie<T>::Trie(const Trie<T>& other) {
    copy(other);
}

template <typename T>
Trie<T>::Trie(Trie<T>&& other) noexcept {
    move(std::move(other));
}

template <typename T>
Trie<T>::~Trie() {
    free();
}

// Assignment operators

template <typename T>
Trie<T>& Trie<T>::operator=(const Trie<T>& other) {
    free();
    copy(other);
    return *this;
}

template <typename T>
Trie<T>& Trie<T>::operator=(Trie<T>&& other) {
    free();
    move(std::move(other));
    return *this;
}

// Insertion, deletion

template <typename T>
template <typename Container>
void Trie<T>::insert(const Container& sequence) {
    if (root_ == nullptr)
        root_ = new TrieNode<T>(false);
    TrieNode<T>* iterator = root_;
    for (const T& item : sequence) {
        if (iterator->children.count(item) == 0)
            iterator->children.emplace(item, new TrieNode<T>(false, iterator));
        
        iterator = iterator->children[item];
    }

    iterator->isKey = true;
}

template <typename T>
template <typename Container>
void Trie<T>::erase(const Container& sequence) {
    eraseRec(root_, sequence, 0);
}

// Search Methods

template <typename T>
template <typename Container>
bool Trie<T>::contains(const Container& sequence) const {
    return findNode(sequence) != nullptr;
}

template <typename T>
template <typename Container>
bool Trie<T>::containsKey(const Container& sequence) const {
    TrieNode<T>* node = findNode(sequence);
    return node != nullptr && node->isKey;
}

template <typename T>
template <typename Container>
TrieNodeIt<T, Container> Trie<T>::find(const Container& sequence) const {
    TrieNode<T>* node = findNode(sequence);
    if (!node)
        return TrieNodeIt<T, Container>(nullptr, Container());
    else
        return TrieNodeIt<T, Container>(node, sequence);
}

template <typename T>
template <typename Container>
TrieNodeIt<T, Container> Trie<T>::findKey(const Container& sequence) const {
    TrieNode<T>* node = findNode(sequence);
    if (!(node && node->isKey))
        return TrieNodeIt<T, Container>(nullptr, Container());
    else
        return TrieNodeIt<T, Container>(node, sequence);
}

template <typename T>
template <typename Container, typename ContContainer>
ContContainer Trie<T>::keysWithPrefix(const Container& prefix) const {
    ContContainer results;
    TrieNode<T>* startNode = findNode(prefix);
    Container prefixCopy(prefix);
    collect(startNode, prefixCopy, results);
    return results;
}

// private Utility

// Helpers for constructors and destructor

template <typename T>
void Trie<T>::free() {
    if (root_ != nullptr)
        freeSubtree(root_);
}

template <typename T>
void Trie<T>::copy(const Trie<T>& other) {
    if (other.root_ == nullptr)
        root_ = nullptr;
    else
        root_ = copySubtree(other.root_);
}

template <typename T>
void Trie<T>::move(Trie<T>&& other) noexcept {
    root_ = other.root_;
    other.root_ = nullptr;
}

template <typename T>
void Trie<T>::freeSubtree(TrieNode<T>* subtreeRoot) {
    for (auto& pair : subtreeRoot->children)
        freeSubtree(pair.second);
        
    delete subtreeRoot;
}

template <typename T>
TrieNode<T>* Trie<T>::copySubtree(const TrieNode<T>* subtreeRoot) {
    TrieNode<T>* result = new TrieNode<T>(subtreeRoot->isKey);
    for (const auto& pair : subtreeRoot->children) {
        result->children.emplace(pair.first, copySubtree(pair.second));
        result->children[pair.first]->parent = result;
    }

    return result;
}

// Find and erase helpers

template <typename T>
template <typename Container>
TrieNode<T>* Trie<T>::findNode(const Container& sequence) const {
    if (root_ == nullptr)
        return nullptr;
    TrieNode<T>* iterator = root_;
    for (const T& item : sequence) {
        if (iterator->children.count(item) == 0)
            return nullptr;
        else
            iterator = iterator->children[item];
    }

    return iterator;
}

template <typename T>
template <typename Container>
bool Trie<T>::eraseRec(TrieNode<T>* currentNode, const Container& sequence, int seqIndex) {
    if (seqIndex == sequence.size()) {
        currentNode->isKey = false;
    } else {
        T item = sequence[seqIndex];
        if (currentNode->children.count(item) > 0 && eraseRec(currentNode->children[item], sequence, seqIndex + 1)) {
            delete currentNode->children[item];
            currentNode->children.erase(item);
        }
    }
    return !currentNode->isKey && currentNode->children.size() == 0;
}

// Keys with prefix helper

template <typename T>
template <typename Container, typename ContContainer>
void Trie<T>::collect(TrieNode<T>* node, Container& prefix, ContContainer& results) const {
    if (node->isKey)
        results.push_back(prefix);

    prefix.resize(prefix.size() + 1);
    for (const auto& pair : node->children) {
        prefix[prefix.size() - 1] = pair.first;
        collect(pair.second, prefix, results);
    }

    prefix.resize(prefix.size() - 1);
}