#pragma once

#include "RBTreeNode.h"
#include "BSTBase.h"

template <typename T>
using RBTreeBase = BSTBase<T, RBTreeNode>;

template <typename T>
class RedBlackTree : public RBTreeBase<T> {
   public:
    using iterator = typename RBTreeBase<T>::iterator;
    using Color = typename RBTreeNode<T>::Color;
    RedBlackTree() : RBTreeBase<T>() {}
    RedBlackTree(const RedBlackTree<T>& other) : RBTreeBase<T>(other) {}
    RedBlackTree(RedBlackTree<T>&& other) : RBTreeBase<T>(std::move(other)) {}

    RedBlackTree<T>& operator=(const RedBlackTree<T>& other);
    RedBlackTree<T>& operator=(RedBlackTree<T>&& other);

    void insert(const T& key);

    void erase(const T& key);
    void erase(iterator& it);
    void erase(iterator&& it);

    T extractMin();
    T extractMax();

   protected:
    using RBTreeBase<T>::rotateLeft;
    using RBTreeBase<T>::rotateRight;

   private:
    void erase(RBTreeNode<T>* node);

    void fixColorsAfterInsertion(RBTreeNode<T>* node);
    void fixColorsAfterDeletion(RBTreeNode<T>* replacement, RBTreeNode<T>* replacementParent);
};

// Assignment operators

template <typename T>
RedBlackTree<T>& RedBlackTree<T>::operator=(const RedBlackTree<T>& other) {
    RBTreeBase<T>::operator=(other);
}

template <typename T>
RedBlackTree<T>& RedBlackTree<T>::operator=(RedBlackTree<T>&& other) {
    RBTreeBase<T>::operator=(std::move(other));
}

// Insertion operation

template <typename T>
void RedBlackTree<T>::insert(const T& key) {
    RBTreeNode<T>* insertedNode = RBTreeBase<T>::insertAndReturnNewNode(key);

    fixColorsAfterInsertion(insertedNode);
}

// Deletion operations

template <typename T>
void RedBlackTree<T>::erase(const T& key) {
    RBTreeNode<T>* nodeToDelete = this->findNode(key);
    if (nodeToDelete != nullptr)
        erase(nodeToDelete);
}

template <typename T>
void RedBlackTree<T>::erase(iterator& it) {
    if (this->getPtr(it) != nullptr) {
        erase(this->getPtr(it));
        this->invalidateIterator(it);
    }
}

template <typename T>
void RedBlackTree<T>::erase(iterator&& it) {
    if (this->getPtr(it) != nullptr) {
        erase(this->getPtr(it));
        this->invalidateIterator(it);
    }
}

// Extract Min / Max

template <typename T>
T RedBlackTree<T>::extractMin() {
    iterator minIt = this->min();
    T key = minIt.key();
    erase(minIt);
    return key;
}

template <typename T>
T RedBlackTree<T>::extractMax() {
    iterator maxIt = this->max();
    T key = maxIt.key();
    erase(maxIt);
    return key;
}

// private Utility

template <typename T>
void RedBlackTree<T>::erase(RBTreeNode<T>* toDelete) {
    Color clrOfDeleted = toDelete->color;
    RBTreeNode<T>* replacementParent = toDelete->parent;
    bool noFixUp = toDelete->left == nullptr || toDelete->right == nullptr;
    RBTreeNode<T>* replacement = this->eraseAndReturnReplacement(toDelete);

    Color replacementClr = Color::BLACK;
    if (replacement != nullptr) {
        replacementClr = replacement->color;
        replacement->color = clrOfDeleted;
    }

    if (noFixUp && clrOfDeleted == Color::RED)
        return;

    if (replacementClr == Color::BLACK)
        fixColorsAfterDeletion(replacement, replacementParent);
}

template <typename T>
void RedBlackTree<T>::fixColorsAfterInsertion(RBTreeNode<T>* node) {
    while (node->parent != nullptr && node->parent->color == Color::RED) {
        bool parentIsLeftChild;
        RBTreeNode<T>* parentSibling;
        if (node->parent == node->parent->parent->left.get()) {
            parentSibling = node->parent->parent->right.get();
            parentIsLeftChild = true;
        } else {
            parentSibling = node->parent->parent->left.get();
            parentIsLeftChild = false;
        }

        if (parentSibling != nullptr && parentSibling->color == Color::RED) {
            node->parent->color = Color::BLACK;
            parentSibling->color = Color::BLACK;
            node->parent->parent->color = Color::RED;

            node = node->parent->parent;
        } else {
            if (parentIsLeftChild && node == node->parent->right.get()) {
                node = node->parent;
                rotateLeft(node);
            } else if (!parentIsLeftChild && node == node->parent->left.get()) {
                node = node->parent;
                rotateRight(node);
            }
            node->parent->color = Color::BLACK;
            node->parent->parent->color = Color::RED;

            if (parentIsLeftChild)
                rotateRight(node->parent->parent);
            else
                rotateLeft(node->parent->parent);
        }
    }
    this->root_->color = Color::BLACK;
}

template <typename T>
void RedBlackTree<T>::fixColorsAfterDeletion(RBTreeNode<T>* replacement, RBTreeNode<T>* replacementParent) {
    RBTreeNode<T>* node = replacement;
    RBTreeNode<T>* parent = replacementParent;
    while (parent != nullptr && (node == nullptr || node->color == Color::BLACK)) {
        bool isLeftChild;
        RBTreeNode<T>* sibling;
        if (node == parent->left.get()) {
            sibling = parent->right.get();
            isLeftChild = true;
        } else {
            sibling = parent->left.get();
            isLeftChild = false;
        }

        if (sibling != nullptr && sibling->color == Color::RED) {
            sibling->color = Color::BLACK;
            parent->color = Color::RED;

            if (isLeftChild) {
                rotateLeft(parent);
                sibling = parent->right.get();
            } else {
                rotateRight(parent);
                sibling = parent->left.get();
            }
        }

        if (sibling == nullptr ||
            ((sibling->left == nullptr || sibling->left->color == Color::BLACK) &&
             (sibling->right == nullptr || sibling->right->color == Color::BLACK))) {
            if (sibling != nullptr)
                sibling->color = Color::RED;
            node = parent;
        } else {
            if (isLeftChild && (sibling->right == nullptr || sibling->right->color == Color::BLACK)) {
                sibling->left->color = Color::BLACK;
                sibling->color = Color::RED;
                rotateRight(sibling);

                sibling = parent->right.get();
            } else if (!isLeftChild && (sibling->left == nullptr || sibling->left->color == Color::BLACK)) {
                sibling->right->color = Color::BLACK;
                sibling->color = Color::RED;
                rotateLeft(sibling);

                sibling = parent->left.get();
            }

            sibling->color = parent->color;
            parent->color = Color::BLACK;

            if (isLeftChild) {
                sibling->right->color = Color::BLACK;
                rotateLeft(parent);
            } else {
                sibling->left->color = Color::BLACK;
                rotateRight(parent);
            }

            node = this->root_.get();
            break;
        }

        parent = node == nullptr ? replacementParent : node->parent;
    }
    if (node != nullptr)
        node->color = Color::BLACK;
}