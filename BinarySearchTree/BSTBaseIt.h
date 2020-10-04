#pragma once

template <typename T, template <typename Type> class Node>
class BSTBase;

template <typename T, template <typename Type> class Node>
class BSTBaseIt {
    Node<T>* currentNode_;

    friend class BSTBase<T, Node>;

   public:
    BSTBaseIt() : currentNode_(nullptr) {}
    explicit BSTBaseIt(Node<T>* node) : currentNode_(node) {}

    bool isValid() const;

    bool hasLeftChild() const;
    bool hasRightChild() const;
    bool hasParent() const;

    const T& key() const;

    BSTBaseIt<T, Node>& left();
    BSTBaseIt<T, Node> getLeft() const;

    BSTBaseIt<T, Node>& right();
    BSTBaseIt<T, Node> getRight() const;

    BSTBaseIt<T, Node>& parent();
    BSTBaseIt<T, Node> getParent() const;
};

template <typename T, template <typename Type> class Node>
bool BSTBaseIt<T, Node>::isValid() const {
    return currentNode_ != nullptr;
}

template <typename T, template <typename Type> class Node>
bool BSTBaseIt<T, Node>::hasLeftChild() const {
    if (!isValid())
        throw std::runtime_error("Tried to check left child of null node");
    return currentNode_->left != nullptr;
}

template <typename T, template <typename Type> class Node>
bool BSTBaseIt<T, Node>::hasRightChild() const {
    if (!isValid())
        throw std::runtime_error("Tried to check right child of null node");
    return currentNode_->right != nullptr;
}

template <typename T, template <typename Type> class Node>
bool BSTBaseIt<T, Node>::hasParent() const {
    if (!isValid())
        throw std::runtime_error("Tried to check parent of null node");
    return currentNode_->parent != nullptr;
}

template <typename T, template <typename Type> class Node>
const T& BSTBaseIt<T, Node>::key() const {
    if (!isValid())
        throw std::runtime_error("Tried to get key of null node");
    return currentNode_->key;
}

template <typename T, template <typename Type> class Node>
BSTBaseIt<T, Node>& BSTBaseIt<T, Node>::left() {
    if (!isValid())
        throw std::runtime_error("Tried to move to left node of null node");
    currentNode_ = currentNode_->left.get();
    return *this;
}

template <typename T, template <typename Type> class Node>
BSTBaseIt<T, Node> BSTBaseIt<T, Node>::getLeft() const {
    if (!isValid())
        throw std::runtime_error("Tried to get left child of null node");
    return BSTBaseIt(currentNode_->left.get());
}

template <typename T, template <typename Type> class Node>
BSTBaseIt<T, Node>& BSTBaseIt<T, Node>::right() {
    if (!isValid())
        throw std::runtime_error("Tried to move to right node of null node");
    currentNode_ = currentNode_->right.get();
    return *this;
}

template <typename T, template <typename Type> class Node>
BSTBaseIt<T, Node> BSTBaseIt<T, Node>::getRight() const {
    if (!isValid())
        throw std::runtime_error("Tried to get right child of null node");
    return BSTBaseIt(currentNode_->right.get());
}

template <typename T, template <typename Type> class Node>
BSTBaseIt<T, Node>& BSTBaseIt<T, Node>::parent() {
    if (!isValid())
        throw std::runtime_error("Tried to move to parent of null node");
    currentNode_ = currentNode_->parent;
    return *this;
}

template <typename T, template <typename Type> class Node>
BSTBaseIt<T, Node> BSTBaseIt<T, Node>::getParent() const {
    if (!isValid())
        throw std::runtime_error("Tried to get parent of null node");
    return BSTBaseIt(currentNode_->parent);
}

#include "BSTBase.h"