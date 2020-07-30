#pragma once

#include "Inorder.h"
#include "Postorder.h"
#include "Preorder.h"
#include "TreeNode.h"
#include "TreeNodeIt.h"

// T must have < and > operators (Replace with C++20 concepts)
template <typename T>
class BinarySearchTree {
    std::unique_ptr<TreeNode<T>> root_;

    friend class Inorder<T>;
    friend class Preorder<T>;
    friend class Postorder<T>;

   public:
    enum Traversal {
        INORDER,
        PREORDER,
        POSTORDER
    };

   private:
    Traversal traversal_;

    Inorder<T> inorder;
    Preorder<T> preorder;
    Postorder<T> postorder;

   public:
    using iterator = TreeNodeIt<T>;
    BinarySearchTree(Traversal trav = INORDER) : root_(nullptr), traversal_(trav) {}
    BinarySearchTree(BinarySearchTree<T>& tree);
    BinarySearchTree(BinarySearchTree<T>&& tree);

    BinarySearchTree<T>& operator=(BinarySearchTree<T>& tree);
    BinarySearchTree<T>& operator=(BinarySearchTree<T>&& tree);

    void insert(const T& key);

    void erase(const T& key);
    void erase(iterator it);

    void clear();

    void setTraversal(Traversal trav);

    iterator find(const T& key);

    iterator min();
    iterator max();
    T minKey();
    T maxKey();

    iterator begin() const;
    iterator end() const;

   private:
    void erase(TreeNode<T>* toDelete);

    TreeNode<T>* minSubtree(TreeNode<T>* subTreeRoot) const;
    TreeNode<T>* maxSubtree(TreeNode<T>* subTreeRoot) const;

    std::unique_ptr<TreeNode<T>>& getUnique(TreeNode<T>* node);

    TreeNode<T>* findNode(const T& key);

    void transplant(TreeNode<T>* toDelete, TreeNode<T>* replacement);
    void transplant(TreeNode<T>* toDelete, std::unique_ptr<TreeNode<T>>& replacement);

    const TreeTraversal<T>* enumToTrav(Traversal trav) const;
};

// Constructors

template <typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree<T>& tree) : BinarySearchTree<T>() {
    Traversal trav = tree.traversal_;
    tree.setTraversal(PREORDER);

    for (const T& key : tree)
        insert(key);

    tree.setTraversal(trav);
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree<T>&& tree) : root_(std::move(tree.root_)) {
    tree.root_ = nullptr;
}

// Assignment operators

template <typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(BinarySearchTree<T>& tree) {
    root_ = nullptr;

    Traversal trav = tree.traversal_;
    tree.setTraversal(PREORDER);

    for (const T& key : tree)
        insert(key);

    tree.setTraversal(trav);
}

template <typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(BinarySearchTree<T>&& tree) {
    root_ = std::move(tree.root_);
    tree.root_ = nullptr;

    return *this;
}

// Insertion and deletion functions

template <typename T>
void BinarySearchTree<T>::insert(const T& key) {  // O(h)
    TreeNode<T>* it = root_.get();
    TreeNode<T>* itParent = nullptr;

    while (it != nullptr) {
        itParent = it;

        if (it->key > key)
            it = it->left.get();
        else
            it = it->right.get();
    }

    if (itParent == nullptr)
        root_ = std::make_unique<TreeNode<T>>(key);
    else if (itParent->key > key)
        itParent->left = std::make_unique<TreeNode<T>>(key, itParent);
    else
        itParent->right = std::make_unique<TreeNode<T>>(key, itParent);
}

template <typename T>
void BinarySearchTree<T>::erase(const T& key) {  // O(h)
    TreeNode<T>* toDelete = findNode(key);
    if (toDelete != nullptr)
        erase(toDelete);
}

template <typename T>
void BinarySearchTree<T>::erase(iterator it) {
    if (it != end())
        erase(it.currentNode_);
}

// public Utility

template <typename T>
void BinarySearchTree<T>::clear() {
    root_ = nullptr;
}

template <typename T>
void BinarySearchTree<T>::setTraversal(Traversal trav) {
    traversal_ = trav;
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::find(const T& key) {
    return iterator(findNode(key), *this, enumToTrav(traversal_));
}

// Min / Max functions

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::min() {  // O(h)
    return iterator(minSubtree(root_.get()), *this, enumToTrav(traversal_));
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::max() {  // O(h)
    return iterator(maxSubtree(root_.get()), *this, enumToTrav(traversal_));
}

template <typename T>
T BinarySearchTree<T>::minKey() {
    return minSubtree(root_.get())->key;
}

template <typename T>
T BinarySearchTree<T>::maxKey() {
    return maxSubtree(root_.get())->key;
}

// Begin / End function

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::begin() const {
    return iterator(*this, enumToTrav(traversal_));
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::end() const {
    return iterator(nullptr, *this, enumToTrav(traversal_));
}

// private Utility functions

template <typename T>
void BinarySearchTree<T>::erase(TreeNode<T>* toDelete) {  // O(h)
    if (toDelete->left == nullptr)
        transplant(toDelete, toDelete->right);
    else if (toDelete->right == nullptr)
        transplant(toDelete, toDelete->left);
    else {
        TreeNode<T>* replacement = minSubtree(toDelete->right.get());

        if (replacement->parent != toDelete) {
            TreeNode<T>* rParent = replacement->parent;

            std::unique_ptr<TreeNode<T>> tmp = std::move(getUnique(replacement));  // Save replacement
            replacement = tmp.get();

            rParent->left = std::move(tmp->right);  // This works, because we already know tmp was the left child of its parent
            tmp->right = nullptr;
            if (rParent->left != nullptr)
                rParent->left->parent = rParent;

            replacement->right = std::move(toDelete->right);
            replacement->right->parent = replacement;

            replacement->left = std::move(toDelete->left);
            replacement->left->parent = replacement;

            transplant(toDelete, tmp);
        } else {
            replacement->left = std::move(toDelete->left);
            replacement->left->parent = replacement;
            transplant(toDelete, getUnique(replacement));
        }
    }
}

// private utility

template <typename T>
TreeNode<T>* BinarySearchTree<T>::minSubtree(TreeNode<T>* subTreeRoot) const {  // O(h)
    TreeNode<T>* it = subTreeRoot;

    while (it != nullptr && it->left != nullptr)
        it = it->left.get();

    return it;
}

template <typename T>
TreeNode<T>* BinarySearchTree<T>::maxSubtree(TreeNode<T>* subTreeRoot) const {  // O(h)
    TreeNode<T>* it = subTreeRoot;

    while (it != nullptr && it->right != nullptr)
        it = it->right.get();

    return it;
}

template <typename T>
std::unique_ptr<TreeNode<T>>& BinarySearchTree<T>::getUnique(TreeNode<T>* node) {  // Can't handle nullptr
    if (node->parent == nullptr)
        return root_;
    else if (node == node->parent->left.get())
        return node->parent->left;
    else
        return node->parent->right;
}

template <typename T>
TreeNode<T>* BinarySearchTree<T>::findNode(const T& key) {  // O(h)
    TreeNode<T>* it = root_.get();

    while (it != nullptr && it->key != key) {
        if (it->key > key)
            it = it->left.get();
        else
            it = it->right.get();
    }

    return it;
}

template <typename T>
void BinarySearchTree<T>::transplant(TreeNode<T>* toDelete, TreeNode<T>* replacement) {  // O(1) Do not use, it is shit
    std::unique_ptr<TreeNode<T>> replacementUnique = (replacement != nullptr) ? std::move(getUnique(replacement)) : nullptr;
    replacement = replacementUnique.get();

    if (replacement != nullptr)
        replacement->parent = toDelete->parent;

    if (toDelete->parent == nullptr)
        root_ = std::move(replacementUnique);
    else if (toDelete == toDelete->parent->left.get())
        toDelete->parent->left = std::move(replacementUnique);
    else
        toDelete->parent->right = std::move(replacementUnique);
}

template <typename T>
void BinarySearchTree<T>::transplant(TreeNode<T>* toDelete, std::unique_ptr<TreeNode<T>>& replacement) {  // O(1)
    if (replacement != nullptr)
        replacement->parent = toDelete->parent;

    if (toDelete->parent == nullptr)
        root_ = std::move(replacement);
    else if (toDelete == toDelete->parent->left.get())
        toDelete->parent->left = std::move(replacement);
    else
        toDelete->parent->right = std::move(replacement);
}

template <typename T>
const TreeTraversal<T>* BinarySearchTree<T>::enumToTrav(Traversal trav) const {
    switch (trav) {
        case INORDER:
            return &inorder;
        case PREORDER:
            return &preorder;
        case POSTORDER:
            return &postorder;
    }
}
