#pragma once

#include <vector>

#include "TreeNode.h"
#include "TreeNodeIt.h"

// T must have < and > operators (Replace with C++20 concepts)
template <typename T>
class BinarySearchTree {
    std::unique_ptr<TreeNode<T>> root_;

   public:
    using iterator = TreeNodeIt<T>;
    BinarySearchTree() : root_(nullptr) {}
    BinarySearchTree(const BinarySearchTree<T>& tree);
    BinarySearchTree(BinarySearchTree<T>&& tree) noexcept;

    BinarySearchTree<T>& operator=(const BinarySearchTree<T>& tree);
    BinarySearchTree<T>& operator=(BinarySearchTree<T>&& tree);

    bool operator==(const BinarySearchTree<T>& other) const;
    bool operator!=(const BinarySearchTree<T>& other) const;

    void insert(const T& key);

    void erase(const T& key);
    // We need to differentiate between an lvalue and an rvalue here, because we have to invalidate the pointer of the iterator
    void erase(iterator& it);
    void erase(iterator&& it);

    void clear();

    template <class Container>
    Container inorder() const;
    template <class Container>
    Container preorder() const;
    template <class Container>
    Container postorder() const;

    bool isEmpty() const;

    size_t computeHeight() const;
    size_t computeSize() const;

    iterator find(const T& key) const;

    iterator min() const;
    iterator max() const;
    T minKey() const;
    T maxKey() const;
    T extractMax();
    T extractMin();

    iterator root() const;

   protected:
    std::unique_ptr<TreeNode<T>>& getUnique(TreeNode<T>* node);

    TreeNode<T>* getPtr(iterator it);  // Think about a better way that does not expose the whole tree to subclasses
    void invalidateIterator(iterator& it);

    TreeNode<T>* insertAndReturnNewNode(const T& key);  // Change Name!

    void rotateLeft(TreeNode<T>* node);
    void rotateRight(TreeNode<T>* node);

    TreeNode<T>* findNode(const T& key) const;

    bool subtreeEqual(TreeNode<T>* node, TreeNode<T>* otherNode) const;

    TreeNode<T>* subtreeMin(TreeNode<T>* subTreeRoot) const;
    TreeNode<T>* subtreeMax(TreeNode<T>* subTreeRoot) const;

    void transplant(TreeNode<T>* toDelete, TreeNode<T>* replacement);
    void transplant(TreeNode<T>* toDelete, std::unique_ptr<TreeNode<T>>& replacement);

   private:
    void erase(TreeNode<T>* toDelete);

    size_t subtreeHeight(TreeNode<T>* subTreeRoot) const;
    size_t subtreeSize(TreeNode<T>* subTreeRoot) const;

    template <class Container>
    void subtreeInorder(TreeNode<T>* subtreeRoot, Container& trav, size_t& currentIndex) const;
    template <class Container>
    void subtreePreorder(TreeNode<T>* subtreeRoot, Container& trav, size_t& currentIndex) const;
    template <class Container>
    void subtreePostorder(TreeNode<T>* subtreeRoot, Container& trav, size_t& currentIndex) const;
};

// Constructors

template <typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree<T>& tree) : BinarySearchTree<T>() {
    auto preorderVec = tree.preorder<std::vector<int>>();

    for (const T& item : preorderVec)
        insert(item);
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree<T>&& tree) noexcept : root_(std::move(tree.root_)) {
    tree.root_ = nullptr;
}

// Assignment operators

template <typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree<T>& tree) {
    clear();

    auto preorderVec = tree.preorder<std::vector<int>>();

    for (const T& item : preorderVec)
        insert(item);
}

template <typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(BinarySearchTree<T>&& tree) {
    root_ = std::move(tree.root_);
    tree.root_ = nullptr;

    return *this;
}

// Comparision operators

template <typename T>
bool BinarySearchTree<T>::operator==(const BinarySearchTree<T>& other) const {
    return subtreeEqual(root_.get(), other.root_.get());
}

template <typename T>
bool BinarySearchTree<T>::operator!=(const BinarySearchTree<T>& other) const {
    return !(*this == other);
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
void BinarySearchTree<T>::erase(iterator& it) {
    if (it.currentNode_ != nullptr) {
        erase(it.currentNode_);
        invalidateIterator(it);
    }
}

template <typename T>
void BinarySearchTree<T>::erase(iterator&& it) {
    if (it.currentNode_ != nullptr) {
        erase(it.currentNode_);
        invalidateIterator(it);
    }
}

// public Utility

template <typename T>
void BinarySearchTree<T>::clear() {
    root_ = nullptr;
}

// Traversals

template <typename T>
template <class Container>
Container BinarySearchTree<T>::inorder() const {
    Container result(computeSize());
    size_t currentIndex = 0;
    subtreeInorder(root_.get(), result, currentIndex);
    return result;
}

template <typename T>
template <class Container>
Container BinarySearchTree<T>::preorder() const {
    Container result(computeSize());
    size_t currentIndex = 0;
    subtreePreorder(root_.get(), result, currentIndex);
    return result;
}

template <typename T>
template <class Container>
Container BinarySearchTree<T>::postorder() const {
    Container result(computeSize());
    size_t currentIndex = 0;
    subtreePostorder(root_.get(), result, currentIndex);
    return result;
}

template <typename T>
bool BinarySearchTree<T>::isEmpty() const {
    return root_ == nullptr;
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
    return iterator(findNode(key));
}

// Min / Max functions

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::min() const {  // O(h)
    return iterator(subtreeMin(root_.get()));
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::max() const {  // O(h)
    return iterator(subtreeMax(root_.get()));
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
    T key = minIt.key();
    erase(minIt);
    return key;
}

template <typename T>
T BinarySearchTree<T>::extractMax() {
    iterator maxIt = max();
    T key = maxIt.key();
    erase(maxIt);
    return key;
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::root() const {
    return iterator(root_.get());
}

// Protected utility functions

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
TreeNode<T>* BinarySearchTree<T>::getPtr(iterator it) {
    return it.currentNode_;
}

template <typename T>
void BinarySearchTree<T>::invalidateIterator(iterator& it) {
    it.currentNode_ = nullptr;
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

    bool isLeftChild = false;
    if (node->parent != nullptr && node == node->parent->left.get())
        isLeftChild = true;

    std::unique_ptr<TreeNode<T>> nodeTmp = std::move(getUnique(node));
    if (node->parent == nullptr)
        root_ = std::move(rightChild);
    else if (isLeftChild)
        node->parent->left = std::move(rightChild);
    else
        node->parent->right = std::move(rightChild);

    rightChildPtr->left = std::move(nodeTmp);
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

    bool isLeftChild = false;
    if (node->parent != nullptr && node == node->parent->left.get())
        isLeftChild = true;

    std::unique_ptr<TreeNode<T>> nodeTmp = std::move(getUnique(node));
    if (node->parent == nullptr)
        root_ = std::move(leftChild);
    else if (isLeftChild)
        node->parent->left = std::move(leftChild);
    else
        node->parent->right = std::move(leftChild);

    leftChildPtr->right = std::move(nodeTmp);
    leftChildPtr->parent = node->parent;
    node->parent = leftChildPtr;
}

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
bool BinarySearchTree<T>::subtreeEqual(TreeNode<T>* node, TreeNode<T>* otherNode) const {
    if (node == nullptr && otherNode == nullptr)
        return true;
    else if (node == nullptr && otherNode != nullptr || node != nullptr && otherNode == nullptr)
        return false;
    else if (node->key != otherNode->key)
        return false;
    else
        return subtreeEqual(node->left.get(), otherNode->left.get()) && subtreeEqual(node->right.get(), otherNode->right.get());
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
template <class Container>
void BinarySearchTree<T>::subtreeInorder(TreeNode<T>* subtreeRoot, Container& trav, size_t& currentIndex) const {
    if (subtreeRoot != nullptr) {
        subtreeInorder(subtreeRoot->left.get(), trav, currentIndex);
        trav[currentIndex] = subtreeRoot->key;
        ++currentIndex;
        subtreeInorder(subtreeRoot->right.get(), trav, currentIndex);
    }
}

template <typename T>
template <class Container>
void BinarySearchTree<T>::subtreePreorder(TreeNode<T>* subtreeRoot, Container& trav, size_t& currentIndex) const {
    if (subtreeRoot != nullptr) {
        trav[currentIndex] = subtreeRoot->key;
        ++currentIndex;
        subtreePreorder(subtreeRoot->left.get(), trav, currentIndex);
        subtreePreorder(subtreeRoot->right.get(), trav, currentIndex);
    }
}

template <typename T>
template <class Container>
void BinarySearchTree<T>::subtreePostorder(TreeNode<T>* subtreeRoot, Container& trav, size_t& currentIndex) const {
    if (subtreeRoot != nullptr) {
        subtreePostorder(subtreeRoot->left.get(), trav, currentIndex);
        subtreePostorder(subtreeRoot->right.get(), trav, currentIndex);
        trav[currentIndex] = subtreeRoot->key;
        ++currentIndex;
    }
}
