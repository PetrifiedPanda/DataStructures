#pragma once

#include <memory>

// These macros should be used inside the public part of a class definition 
// to make these nodes usable as template  parameters of BSTBase

// Use this if the node type has no other parameters that should always be initialized
#define BasicTreeNode(NodeType, T) \
    T key; \
    std::unique_ptr<NodeType<T>> left; \
    std::unique_ptr<NodeType<T>> right; \
    NodeType<T>* parent; \
    \
    NodeType(const T& key) : key(key), left(nullptr), right(nullptr), parent(nullptr) {} \
    NodeType(const T& key, NodeType<T>* parent) : key(key), left(nullptr), right(nullptr), parent(parent) {} \
    NodeType(const NodeType<T>& other) : key(other.key), left(nullptr), right(nullptr), parent(nullptr) {}

// The varargs should be used to initialize other members of the node and a copy constructor must be provided (analogous to the one in BasicTreeNode())
#define TreeNode(NodeType, T, ...) \
    T key; \
    std::unique_ptr<NodeType<T>> left; \
    std::unique_ptr<NodeType<T>> right; \
    NodeType<T>* parent; \
    \
    NodeType(const T& key) : __VA_ARGS__, key(key), left(nullptr), right(nullptr), parent(nullptr) {} \
    NodeType(const T& key, NodeType<T>* parent) : __VA_ARGS__, key(key), left(nullptr), right(nullptr), parent(parent) {}
