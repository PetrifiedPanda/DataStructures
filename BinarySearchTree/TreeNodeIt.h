#pragma once

#include "TreeNode.h"
#include "TreeTraversal.h"

template <typename T>
class BinarySearchTree;

template <typename T>
class TreeNodeIt {
    TreeNode<T>* currentNode_;
    const BinarySearchTree<T>& tree_;
    const TreeTraversal<T>* traversal_;

    friend class BinarySearchTree<T>;

   public:
    TreeNodeIt(TreeNode<T>* node, const BinarySearchTree<T>& tree, const TreeTraversal<T>* traversal) : currentNode_(node), tree_(tree), traversal_(traversal) {}
    TreeNodeIt(const BinarySearchTree<T>& tree, const TreeTraversal<T>* traversal) : tree_(tree), traversal_(traversal) {
        currentNode_ = traversal_->startNode(tree_);
    }

    TreeNodeIt<T>& operator++();

    TreeNodeIt<T> operator+(int offset);

    const T& operator*() const;

    bool operator==(const TreeNodeIt<T>& other) const;
    bool operator!=(const TreeNodeIt<T>& other) const;
};

// Increment / Decrement operators

template <typename T>
TreeNodeIt<T>& TreeNodeIt<T>::operator++() {  // O(h)
    if (currentNode_ != nullptr)
        currentNode_ = traversal_->getSuccessor(currentNode_, tree_);
    return *this;
}

// Addition / Subtraction operators

template <typename T>
TreeNodeIt<T> TreeNodeIt<T>::operator+(int offset) {  // O(h * offset)
    TreeNodeIt<T> result(currentNode_, tree_, traversal_);

    for (int i = 0; i < offset; ++i)
        ++result;

    return result;
}

// Dereference operators

template <typename T>
const T& TreeNodeIt<T>::operator*() const {
    return currentNode_->key;
}

// Comparision operators

template <typename T>
bool TreeNodeIt<T>::operator==(const TreeNodeIt<T>& other) const {
    return currentNode_ == other.currentNode_;
}

template <typename T>
bool TreeNodeIt<T>::operator!=(const TreeNodeIt<T>& other) const {
    return !(*this == other);
}

#include "BinarySearchTree.h"