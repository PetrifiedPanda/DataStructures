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
    if (!isValid())
        throw std::runtime_error("Tried to check left child of null node");
    return currentNode_->left != nullptr;
}

template <typename T>
bool TreeNodeIt<T>::hasRightChild() const {
    if (!isValid())
        throw std::runtime_error("Tried to check right child of null node");
    return currentNode_->right != nullptr;
}

template <typename T>
bool TreeNodeIt<T>::hasParent() const {
    if (!isValid())
        throw std::runtime_error("Tried to check parent of null node");
    return currentNode_->parent != nullptr;
}

template <typename T>
const T& TreeNodeIt<T>::key() const {
    if (!isValid())
        throw std::runtime_error("Tried to get key of null node");
    return currentNode_->key;
}

template <typename T>
TreeNodeIt<T>& TreeNodeIt<T>::left() {
    if (!isValid())
        throw std::runtime_error("Tried to move to left node of null node");
    currentNode_ = currentNode_->left.get();
}

template <typename T>
TreeNodeIt<T> TreeNodeIt<T>::getLeft() const {
    if (!isValid())
        throw std::runtime_error("Tried to get left child of null node");
    return TreeNodeIt(currentNode_->left.get());
}

template <typename T>
TreeNodeIt<T>& TreeNodeIt<T>::right() {
    if (!isValid())
        throw std::runtime_error("Tried to move to right node of null node");
    currentNode_ = currentNode_->right.get();
}

template <typename T>
TreeNodeIt<T> TreeNodeIt<T>::getRight() const {
    if (!isValid())
        throw std::runtime_error("Tried to get right child of null node");
    return TreeNodeIt(currentNode_->right.get());
}

template <typename T>
TreeNodeIt<T>& TreeNodeIt<T>::parent() {
    if (!isValid())
        throw std::runtime_error("Tried to move to parent of null node");
    currentNode_ = currentNode_->parent;
}

template <typename T>
TreeNodeIt<T> TreeNodeIt<T>::getParent() const {
    if (!isValid())
        throw std::runtime_error("Tried to get parent of null node");
    return TreeNodeIt(currentNode_->parent);
}

#include "BinarySearchTree.h"
