#pragma once

#include "TreeNode.h"

template <typename T>
class BinarySearchTree;

// These need more checks for nullptr

template <typename T>
class TreeNodeIt {
    TreeNode<T>* currentNode_;

    friend class BinarySearchTree<T>;

   public:
    TreeNodeIt() : currentNode_(nullptr) {}
    explicit TreeNodeIt(TreeNode<T>* node) : currentNode_(node) {}

    bool isValid() const;

    bool hasLeftChild() const;
    bool hasRightChild() const;
    bool hasParent() const;

    const T& key() const;

    TreeNodeIt<T>& left();
    TreeNodeIt<T> getLeft() const;

    TreeNodeIt<T>& right();
    TreeNodeIt<T> getRight() const;

    TreeNodeIt<T>& parent();
    TreeNodeIt<T> getParent() const;
};

template <typename T>
bool TreeNodeIt<T>::isValid() const {
    return currentNode_ != nullptr;
}

template <typename T>
bool TreeNodeIt<T>::hasLeftChild() const {
    return currentNode_->left != nullptr;
}

template <typename T>
bool TreeNodeIt<T>::hasRightChild() const {
    return currentNode_->right != nullptr;
}

template <typename T>
bool TreeNodeIt<T>::hasParent() const {
    return currentNode_->parent != nullptr;
}

template <typename T>
const T& TreeNodeIt<T>::key() const {
    return currentNode_->key;
}

template <typename T>
TreeNodeIt<T>& TreeNodeIt<T>::left() {
    currentNode_ = currentNode_->left.get();
}

template <typename T>
TreeNodeIt<T> TreeNodeIt<T>::getLeft() const {
    return TreeNodeIt(currentNode_->left.get());
}

template <typename T>
TreeNodeIt<T>& TreeNodeIt<T>::right() {
    currentNode_ = currentNode_->right.get();
}

template <typename T>
TreeNodeIt<T> TreeNodeIt<T>::getRight() const {
    return TreeNodeIt(currentNode_->right.get());
}

template <typename T>
TreeNodeIt<T>& TreeNodeIt<T>::parent() {
    currentNode_ = currentNode_->parent;
}

template <typename T>
TreeNodeIt<T> TreeNodeIt<T>::getParent() const {
    return TreeNodeIt(currentNode_->parent);
}

#include "BinarySearchTree.h"
