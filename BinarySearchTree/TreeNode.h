#pragma once

#include <memory>

template <typename T>
class TreeNode {
   public:
    T key;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
    TreeNode<T>* parent;

    TreeNode(T key) : key(key), left(nullptr), right(nullptr), parent(nullptr) {}
    TreeNode(T key, TreeNode<T>* p) : key(key), parent(p) {}
};