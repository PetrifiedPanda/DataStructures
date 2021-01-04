#pragma once

#include "BSTBase.h"

#include "TreeNode.h"

template <typename T>
class RBTreeNode {
   public:
    enum Color {
        BLACK,
        RED
    };

    Color color;
    TreeNode(RBTreeNode, T, color(RED));
    RBTreeNode(const RBTreeNode<T>& other) : RBTreeNode<T>(other.key) {
        color = other.color;
    }
};

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
    void fixDoubleBlack(RBTreeNode<T>* node);
};

// Assignment operators

template <typename T>
RedBlackTree<T>& RedBlackTree<T>::operator=(const RedBlackTree<T>& other) {
    RBTreeBase<T>::operator=(other);
    return *this;
}

template <typename T>
RedBlackTree<T>& RedBlackTree<T>::operator=(RedBlackTree<T>&& other) {
    RBTreeBase<T>::operator=(std::move(other));
    return *this;
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
        it.invalidate();
    }
}

template <typename T>
void RedBlackTree<T>::erase(iterator&& it) {
    if (this->getPtr(it) != nullptr) {
        erase(this->getPtr(it));
        it.invalidate();
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
    RBTreeNode<T>* replacement = this->findReplacement(toDelete);
    bool bothBlack = (replacement == nullptr || replacement->color == Color::BLACK) && (toDelete->color == Color::BLACK);

    RBTreeNode<T>* parent = toDelete->parent;

    if (replacement == nullptr) {
        if (toDelete == this->root_.get()) {
           this->root_ = nullptr;
        } else {
            if (bothBlack) {
               fixDoubleBlack(toDelete);
            } else {
                if (toDelete == toDelete->parent->left.get() && toDelete->parent->right != nullptr) {
                    toDelete->parent->right->color = Color::RED;
                } else if (toDelete == toDelete->parent->right.get() && toDelete->parent->left != nullptr) {
                    toDelete->parent->left->color = Color::RED;
                }
            }

            if (toDelete == parent->left.get())
                parent->left = nullptr;
            else
                parent->right = nullptr;
        }
    } else if (toDelete->left == nullptr || toDelete->right == nullptr) {
        // Because toDelete has only one child, that child must be replacement
        if (toDelete == this->root_.get()) {
           toDelete->key = replacement->key;
           toDelete->left =  nullptr;
           toDelete->right = nullptr;
        } else {
            bool isLeft = toDelete == toDelete->parent->left.get();

            if (isLeft)
               parent->left = std::move(this->getUnique(replacement));
            else
                parent->right = std::move(this->getUnique(replacement));
            
            replacement->parent = parent;

            if (bothBlack)
                fixDoubleBlack(replacement);
            else
                replacement->color = Color::BLACK;
        }
    } else {
        std::swap(toDelete->key, replacement->key);
        erase(replacement);
    }
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
void RedBlackTree<T>::fixDoubleBlack(RBTreeNode<T>* node) {
    while (node != this->root_.get()) {
        RBTreeNode<T>* parent = node->parent;

        if ((node == parent->left.get() && parent->right == nullptr) || 
            (node == parent->right.get() && parent->left == nullptr)) {
            node = parent;
        } else {
            RBTreeNode<T>* sibling = node == parent->left.get() ? parent->right.get() : parent->left.get();
            if (sibling->color == Color::RED) {
                parent->color = Color::RED;
                sibling->color = Color::BLACK;

                if (sibling == parent->left.get())
                    this->rotateRight(parent);
                else
                    this->rotateLeft(parent);
            } else {
                if ((sibling->left != nullptr && sibling->left->color == Color::RED) || 
                    (sibling->right != nullptr && sibling->right->color == Color::RED)) {
                
                    if (sibling->left != nullptr && sibling->left->color == Color::RED) {
                        if (sibling == sibling->parent->left.get()) {
                            sibling->left->color = sibling->color;
                            sibling->color = parent->color;
                            this->rotateRight(parent);
                        } else {
                            sibling->left->color = parent->color;
                            this->rotateRight(sibling);
                            this->rotateLeft(parent);
                        }
                    } else {
                        if (sibling == sibling->parent->left.get()) {
                            sibling->right->color = parent->color;
                            this->rotateLeft(sibling);
                            this->rotateRight(parent);
                        } else {
                            sibling->right->color = sibling->color;
                            sibling->color = parent->color;
                            this->rotateLeft(parent);
                        }
                    }
                    parent->color = Color::BLACK;
                    break;
                } else {
                    sibling->color = Color::RED;
                    if (parent->color = Color::BLACK)
                        node = parent;
                    else {
                        parent->color = Color::BLACK;
                        break;
                    }
                }
            }
        }
    }
}