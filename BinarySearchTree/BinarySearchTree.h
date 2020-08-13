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
    // The setTraversal() function should stay non-const while every const function that modifies this should reset it after usage
    mutable Traversal traversal_;

    Inorder<T> inorder;
    Preorder<T> preorder;
    Postorder<T> postorder;

   public:
    using iterator = TreeNodeIt<T>;
    BinarySearchTree(Traversal trav = INORDER) : root_(nullptr), traversal_(trav) {}
    BinarySearchTree(const BinarySearchTree<T>& tree);
    BinarySearchTree(BinarySearchTree<T>&& tree) noexcept;

    BinarySearchTree<T>& operator=(const BinarySearchTree<T>& tree);
    BinarySearchTree<T>& operator=(BinarySearchTree<T>&& tree);

    void insert(const T& key);

    void erase(const T& key);
    void erase(iterator it);

    template <typename Function>
    void forEach(const Function& func, Traversal trav) const {
        Traversal prevTrav = traversal_;
        traversal_ = trav;

        for (const T& item : *this)
            func(item);

        traversal_ = prevTrav;
    }

    template <typename ReturnType, typename Function>
    ReturnType doWithTraversal(const Function& func, Traversal trav) const {
        Traversal prevTrav = traversal_;
        traversal_ = trav;

        ReturnType result = func(*this);

        traversal_ = prevTrav;
        return result;
    }

    template <typename Function>
    void doWithTraversal(const Function& func, Traversal trav) const {
        Traversal prevTrav = traversal_;
        traversal_ = trav;

        func(*this);

        traversal_ = prevTrav;
    }

    void clear();

    void setTraversal(Traversal trav);

    Traversal traversal() const;

    size_t computeHeight() const;
    size_t computeSize() const;

    iterator find(const T& key) const;

    iterator min() const;
    iterator max() const;
    T minKey() const;
    T maxKey() const;
    T extractMax();
    T extractMin();

    iterator begin() const;
    iterator end() const;

   protected:
    TreeNode<T>* insertAndReturnNewNode(const T& key);  // Change Name!

    void rotateLeft(TreeNode<T>* node);
    void rotateRight(TreeNode<T>* node);

    TreeNode<T>* findNode(const T& key) const;

    const TreeTraversal<T>* getTraversal() const;

   private:
    void erase(TreeNode<T>* toDelete);

    size_t subtreeHeight(TreeNode<T>* subTreeRoot) const;
    size_t subtreeSize(TreeNode<T>* subTreeRoot) const;

    TreeNode<T>* subtreeMin(TreeNode<T>* subTreeRoot) const;
    TreeNode<T>* subtreeMax(TreeNode<T>* subTreeRoot) const;

    std::unique_ptr<TreeNode<T>>& getUnique(TreeNode<T>* node);

    void transplant(TreeNode<T>* toDelete, TreeNode<T>* replacement);
    void transplant(TreeNode<T>* toDelete, std::unique_ptr<TreeNode<T>>& replacement);
};

// Constructors

template <typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree<T>& tree) : BinarySearchTree<T>() {
    doWithTraversal([&](const BinarySearchTree<T>& tree) {
        for (const T& key : tree)
            insert(key);
    },
                    PREORDER);
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree<T>&& tree) noexcept : root_(std::move(tree.root_)) {
    tree.root_ = nullptr;
}

// Assignment operators

template <typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree<T>& tree) {
    clear();

    doWithTraversal([&](const BinarySearchTree<T>& tree) {
        for (const T& key : tree)
            insert(key);
    },
                    PREORDER);
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
    insertAndReturnNewNode(key);
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
typename BinarySearchTree<T>::Traversal BinarySearchTree<T>::traversal() const {
    return traversal_;
}

template <typename T>
size_t BinarySearchTree<T>::computeHeight() const {
    return subtreeHeight(root_.get());
}

template <typename T>
size_t BinarySearchTree<T>::computeSize() const {
    return subtreeSize(root_.get());
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::find(const T& key) const {
    return iterator(findNode(key), *this, getTraversal());
}

// Min / Max functions

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::min() const {  // O(h)
    return iterator(subtreeMin(root_.get()), *this, getTraversal());
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::max() const {  // O(h)
    return iterator(subtreeMax(root_.get()), *this, getTraversal());
}

template <typename T>
T BinarySearchTree<T>::minKey() const {
    return subtreeMin(root_.get())->key;
}

template <typename T>
T BinarySearchTree<T>::maxKey() const {
    return subtreeMax(root_.get())->key;
}

template <typename T>
T BinarySearchTree<T>::extractMin() {
    iterator minIt = min();
    T key = *minIt;
    erase(minIt);
    return key;
}

template <typename T>
T BinarySearchTree<T>::extractMax() {
    iterator maxIt = max();
    T key = *maxIt;
    erase(maxIt);
    return key;
}

// Begin / End functions

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::begin() const {
    return iterator(*this, getTraversal());
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::end() const {
    return iterator(nullptr, *this, getTraversal());
}

template <typename T>
TreeNode<T>* BinarySearchTree<T>::insertAndReturnNewNode(const T& key) {
    TreeNode<T>* it = root_.get();
    TreeNode<T>* itParent = nullptr;

    while (it != nullptr) {
        itParent = it;

        if (it->key > key)
            it = it->left.get();
        else
            it = it->right.get();
    }

    if (itParent == nullptr) {
        root_ = std::make_unique<TreeNode<T>>(key);
        return root_.get();
    } else if (itParent->key > key) {
        itParent->left = std::make_unique<TreeNode<T>>(key, itParent);
        return itParent->left.get();
    } else {
        itParent->right = std::make_unique<TreeNode<T>>(key, itParent);
        return itParent->right.get();
    }
}

// Rotations

template <typename T>
void BinarySearchTree<T>::rotateLeft(TreeNode<T>* node) {
    std::unique_ptr<TreeNode<T>> rightChild = std::move(node->right);
    TreeNode<T>* rightChildPtr = rightChild.get();

    if (rightChild->left != nullptr)
        rightChild->left->parent = node;
    node->right = std::move(rightChild->left);

    if (node->parent == nullptr) {
        std::unique_ptr<TreeNode<T>> nodeTmp = std::move(getUnique(node));
        root_ = std::move(rightChild);
        root_->left = std::move(nodeTmp);
    } else {
        TreeNode<T>* nodeParent = node->parent;
        bool isLeftChild = false;
        if (node == nodeParent->left.get())
            isLeftChild = true;

        std::unique_ptr<TreeNode<T>> nodeTmp = std::move(getUnique(node));
        if (isLeftChild)
            nodeParent->left = std::move(rightChild);
        else
            nodeParent->right = std::move(rightChild);

        rightChildPtr->left = std::move(nodeTmp);
    }
    rightChildPtr->parent = node->parent;
    node->parent = rightChildPtr;
}

template <typename T>
void BinarySearchTree<T>::rotateRight(TreeNode<T>* node) {
    std::unique_ptr<TreeNode<T>> leftChild = std::move(node->left);
    TreeNode<T>* leftChildPtr = leftChild.get();

    if (leftChild->right != nullptr)
        leftChild->right->parent = node;
    node->left = std::move(leftChild->right);

    if (node->parent == nullptr) {
        std::unique_ptr<TreeNode<T>> nodeTmp = std::move(getUnique(node));
        root_ = std::move(leftChild);
        root_->right = std::move(nodeTmp);
    } else {
        TreeNode<T>* nodeParent = node->parent;
        bool isLeftChild = false;
        if (node == nodeParent->left.get())
            isLeftChild = true;

        std::unique_ptr<TreeNode<T>> nodeTmp = std::move(getUnique(node));
        if (isLeftChild)
            nodeParent->left = std::move(leftChild);
        else
            nodeParent->right = std::move(leftChild);

        leftChildPtr->right = std::move(nodeTmp);
    }
    leftChildPtr->parent = node->parent;
    node->parent = leftChildPtr;
}

// protected utility

template <typename T>
TreeNode<T>* BinarySearchTree<T>::findNode(const T& key) const {  // O(h)
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
const TreeTraversal<T>* BinarySearchTree<T>::getTraversal() const {
    switch (traversal_) {
        case INORDER:
            return &inorder;
        case PREORDER:
            return &preorder;
        case POSTORDER:
            return &postorder;
    }
}

// private Utility functions

template <typename T>
void BinarySearchTree<T>::erase(TreeNode<T>* toDelete) {  // O(h)
    if (toDelete->left == nullptr)
        transplant(toDelete, toDelete->right);
    else if (toDelete->right == nullptr)
        transplant(toDelete, toDelete->left);
    else {
        TreeNode<T>* replacement = subtreeMin(toDelete->right.get());

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
size_t BinarySearchTree<T>::subtreeHeight(TreeNode<T>* subtreeRoot) const {
    if (subtreeRoot == nullptr)
        return 0;

    return std::max(subtreeHeight(subtreeRoot->right.get()), subtreeHeight(subtreeRoot->left.get())) + 1;
}

template <typename T>
size_t BinarySearchTree<T>::subtreeSize(TreeNode<T>* subtreeRoot) const {
    if (subtreeRoot == nullptr)
        return 0;

    return subtreeSize(subtreeRoot->right.get()) + subtreeSize(subtreeRoot->left.get()) + 1;
}

template <typename T>
TreeNode<T>* BinarySearchTree<T>::subtreeMin(TreeNode<T>* subTreeRoot) const {  // O(h)
    TreeNode<T>* it = subTreeRoot;

    while (it != nullptr && it->left != nullptr)
        it = it->left.get();

    return it;
}

template <typename T>
TreeNode<T>* BinarySearchTree<T>::subtreeMax(TreeNode<T>* subTreeRoot) const {  // O(h)
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
void BinarySearchTree<T>::transplant(TreeNode<T>* toDelete, TreeNode<T>* replacement) {  // This does not work, and I still need to find out why
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
