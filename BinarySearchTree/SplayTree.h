#include "BinarySearchTree.h"

template <typename T>
class SplayTree : public BinarySearchTree<T> {
   public:
    using iterator = typename BinarySearchTree<T>::iterator;
    using Traversal = typename BinarySearchTree<T>::Traversal;  // Maybe make Traversal an enum class declared outside BinarySearchTree

    SplayTree(Traversal trav = BinarySearchTree<T>::INORDER) : BinarySearchTree<T>(trav) {}
    SplayTree(const SplayTree<T>& tree) : BinarySearchTree<T>(tree) {}
    SplayTree(const BinarySearchTree<T>& tree) : BinarySearchTree<T>(tree) {}
    SplayTree(SplayTree<T>&& tree) : BinarySearchTree<T>(std::move(tree)) {}
    SplayTree(BinarySearchTree<T>&& tree) : BinarySearchTree<T>(std::move(tree)) {}

    SplayTree<T>& operator=(const SplayTree<T>& tree);
    SplayTree<T>& operator=(SplayTree<T>&& tree);

    void insert(const T& key);

    iterator find(const T& key);

   private:
    void splay(TreeNode<T>* node);

    void zigZig(TreeNode<T>* node);
    void zigZag(TreeNode<T>* node);

    void zig(TreeNode<T>* node);
};

// Assignment operators

template <typename T>
SplayTree<T>& SplayTree<T>::operator=(const SplayTree<T>& tree) {
    BinarySearchTree<T>::operator=(tree);
}

template <typename T>
SplayTree<T>& SplayTree<T>::operator=(SplayTree<T>&& tree) {
    BinarySearchTree<T>::operator=(std::move(tree));
}

// Insertion operation

template <typename T>
void SplayTree<T>::insert(const T& key) {
    splay(this->insertAndReturnNewNode(key));
}

// Search operation

template <typename T>
typename SplayTree<T>::iterator SplayTree<T>::find(const T& key) {
    TreeNode<T>* keyNode = findNode(key);
    if (keyNode != nullptr)
        splay(keyNode);
    return iterator(keyNode, *this, BinarySearchTree<T>::getTraversal());
}

// Splay operations and helpers

template <typename T>
void SplayTree<T>::splay(TreeNode<T>* node) {
    while (node->parent != nullptr) {
        if (node->parent->parent == nullptr)
            zig(node);
        else if ((node->parent->parent->left != nullptr && node == node->parent->parent->left->left.get()) ||
                 (node->parent->parent->right != nullptr && node == node->parent->parent->right->right.get()))
            zigZig(node);
        else
            zigZag(node);
    }
}

template <typename T>
void SplayTree<T>::zigZig(TreeNode<T>* node) {
    if (node == node->parent->left.get()) {
        this->rotateRight(node->parent->parent);
        this->rotateRight(node->parent);
    } else {
        this->rotateLeft(node->parent->parent);
        this->rotateLeft(node->parent);
    }
}

template <typename T>
void SplayTree<T>::zigZag(TreeNode<T>* node) {
    if (node == node->parent->left.get()) {
        this->rotateRight(node->parent);
        this->rotateLeft(node->parent);
    } else {
        this->rotateLeft(node->parent);
        this->rotateRight(node->parent);
    }
}

template <typename T>
void SplayTree<T>::zig(TreeNode<T>* node) {
    if (node == node->parent->left.get())
        this->rotateRight(node->parent);
    else
        this->rotateLeft(node->parent);
}