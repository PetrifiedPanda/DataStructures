#pragma once

#include <memory>

template <typename T>
class TreeNode {
   public:
    T key;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
    TreeNode<T>* parent;

    TreeNode(const T& key) : key(key), left(nullptr), right(nullptr), parent(nullptr) {}
};