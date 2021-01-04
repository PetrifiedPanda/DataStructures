#include "BinarySearchTree.h"

template <typename T>
class SplayTree : public BinarySearchTree<T> {
   public:
    using iterator = typename BinarySearchTree<T>::iterator;

    SplayTree() : BinarySearchTree<T>() {}
    SplayTree(const SplayTree<T>& tree) : BinarySearchTree<T>(tree) {}
    SplayTree(const BinarySearchTree<T>& tree) : BinarySearchTree<T>(tree) {}
    SplayTree(SplayTree<T>&& tree) : BinarySearchTree<T>(std::move(tree)) {}
    SplayTree(BinarySearchTree<T>&& tree) : BinarySearchTree<T>(std::move(tree)) {}

    SplayTree<T>& operator=(const SplayTree<T>& tree);
    SplayTree<T>& operator=(SplayTree<T>&& tree);

    void insert(const T& key);

    void erase(const T& key);
    void erase(iterator& it);
    void erase(iterator&& it);

    iterator find(const T& key);

   private:
    void erase(TreeNode<T>* node);

    void splay(TreeNode<T>* node);

    void splayUpTo(TreeNode<T>* node, TreeNode<T>* newParent);

    void zigZig(TreeNode<T>* node);
    void zigZag(TreeNode<T>* node);

    void zig(TreeNode<T>* node);
};

// Assignment operators

template <typename T>
SplayTree<T>& SplayTree<T>::operator=(const SplayTree<T>& tree) {
    BinarySearchTree<T>::operator=(tree);
    return *this;
}

template <typename T>
SplayTree<T>& SplayTree<T>::operator=(SplayTree<T>&& tree) {
    BinarySearchTree<T>::operator=(std::move(tree));
    return *this;
}

// Insertion operation

template <typename T>
void SplayTree<T>::insert(const T& key) {
    splay(this->insertAndReturnNewNode(key));
}

// Delete operation

template <typename T>
void SplayTree<T>::erase(const T& key) {
    TreeNode<T>* keyNode = this->findNode(key);
    if (keyNode != nullptr)
        erase(keyNode);
}

template <typename T>
void SplayTree<T>::erase(iterator& it) {
    TreeNode<T>* itNode = this->getPtr(it);
    if (itNode != nullptr) {
        erase(itNode);
        this->invalidateIterator(it);
    }
}

template <typename T>
void SplayTree<T>::erase(iterator&& it) {
    TreeNode<T>* itNode = this->getPtr(it);
    if (itNode != nullptr) {
        erase(itNode);
        this->invalidateIterator(it);
    }
}

// Search operation

template <typename T>
typename SplayTree<T>::iterator SplayTree<T>::find(const T& key) {
    TreeNode<T>* keyNode = this->findNode(key);
    if (keyNode != nullptr)
        splay(keyNode);
    return iterator(keyNode);
}

template <typename T>
void SplayTree<T>::erase(TreeNode<T>* node) {
    splay(node);

    if (node->left == nullptr) {
        this->transplant(node, node->right);
    } else if (node->right == nullptr) {
        this->transplant(node, node->left);
    } else {
        TreeNode<T>* leftMax = this->subtreeMax(node->left.get());
        splayUpTo(leftMax, node);
        leftMax->right = std::move(node->right);
        leftMax->right->parent = leftMax;
        this->transplant(node, this->getUnique(leftMax));
    }
}

// Splay operations and helpers

template <typename T>
void SplayTree<T>::splay(TreeNode<T>* node) {
    splayUpTo(node, nullptr);
}

template <typename T>
void SplayTree<T>::splayUpTo(TreeNode<T>* node, TreeNode<T>* newParent) {
    while (node->parent != newParent) {
        if (node->parent->parent == newParent)
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