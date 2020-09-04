#pragma once

#include <memory>

template <typename T>
class RBTreeNode {
   public:
    enum Color {
        BLACK,
        RED
    };

    T key;
    std::unique_ptr<RBTreeNode> left;
    std::unique_ptr<RBTreeNode> right;
    RBTreeNode<T>* parent;
    Color color;

    RBTreeNode(const T& key) : key(key), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
    RBTreeNode(const T& key, RBTreeNode<T>* parent) : key(key), left(nullptr), right(nullptr), parent(parent), color(RED) {}
};