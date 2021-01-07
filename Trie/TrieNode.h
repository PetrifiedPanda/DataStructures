#pragma once

#include <unordered_map>

template <typename T>
class TrieNode {
   public:
    bool isKey;
    std::unordered_map<T, TrieNode*> children;
    TrieNode<T>* parent;

    TrieNode(bool isKey, TrieNode<T>* parent = nullptr) : isKey(isKey), children(), parent(parent) {}
};