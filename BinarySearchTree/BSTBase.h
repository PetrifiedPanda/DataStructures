#pragma once

#include <vector>

#include "BSTBaseIt.h"

// TODO: Add Node-independent copy (and maybe compare) operator, so trees with different node types can be assigned to each other

// T must have < and > operators (Replace with C++20 concepts)
// Node must basically be a class almost identical to TreeNode, or one that inherits from it (Also replace with C++20 concepts if possible)
template <typename T, template <typename> class Node>
class BSTBase {
   protected:
    std::unique_ptr<Node<T>> root_;

   public:
    using iterator = BSTBaseIt<T, Node>;
    BSTBase() : root_(nullptr) {}
    BSTBase(const BSTBase<T, Node>& tree);
    BSTBase(BSTBase<T, Node>&& tree) noexcept;

    BSTBase<T, Node>& operator=(const BSTBase<T, Node>& tree);
    BSTBase<T, Node>& operator=(BSTBase<T, Node>&& tree);

    bool operator==(const BSTBase<T, Node>& other) const;
    bool operator!=(const BSTBase<T, Node>& other) const;

    void insert(const T& key);

    void erase(const T& key);
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
    std::unique_ptr<Node<T>>& getUnique(Node<T>* node);

    Node<T>* getPtr(iterator it);
    void invalidateIterator(iterator& it);

    Node<T>* insertAndReturnNewNode(const T& key);
    void erase(Node<T>* toDelete);
    Node<T>* findReplacement(Node<T>* node);

    void rotateLeft(Node<T>* node);
    void rotateRight(Node<T>* node);

    Node<T>* findNode(const T& key) const;

    bool subtreeEqual(Node<T>* node, Node<T>* otherNode) const;

    Node<T>* subtreeMin(Node<T>* subTreeRoot) const;
    Node<T>* subtreeMax(Node<T>* subTreeRoot) const;

    void transplant(Node<T>* toDelete, Node<T>* replacement);
    void transplant(Node<T>* toDelete, std::unique_ptr<Node<T>>& replacement);

   private:
    size_t subtreeHeight(Node<T>* subTreeRoot) const;
    size_t subtreeSize(Node<T>* subTreeRoot) const;

    template <class Container>
    void subtreeInorder(Node<T>* subtreeRoot, Container& trav, size_t& currentIndex) const;
    template <class Container>
    void subtreePreorder(Node<T>* subtreeRoot, Container& trav, size_t& currentIndex) const;
    template <class Container>
    void subtreePostorder(Node<T>* subtreeRoot, Container& trav, size_t& currentIndex) const;
};

// Constructors

template <typename T, template <typename> class Node>
BSTBase<T, Node>::BSTBase(const BSTBase<T, Node>& tree) : BSTBase<T, Node>() {
    auto preorderVec = tree.preorder<std::vector<T>>();

    for (const T& item : preorderVec)
        insert(item);
}

template <typename T, template <typename> class Node>
BSTBase<T, Node>::BSTBase(BSTBase<T, Node>&& tree) noexcept : root_(std::move(tree.root_)) {
    tree.root_ = nullptr;
}

// Assignment operators

template <typename T, template <typename> class Node>
BSTBase<T, Node>& BSTBase<T, Node>::operator=(const BSTBase<T, Node>& tree) {
    auto preorderVec = tree.preorder<std::vector<T>>();

    clear();

    for (const T& item : preorderVec)
        insert(item);
}

template <typename T, template <typename> class Node>
BSTBase<T, Node>& BSTBase<T, Node>::operator=(BSTBase<T, Node>&& tree) {
    root_ = std::move(tree.root_);
    tree.root_ = nullptr;

    return *this;
}

// Comparision operators

template <typename T, template <typename> class Node>
bool BSTBase<T, Node>::operator==(const BSTBase<T, Node>& other) const {
    return subtreeEqual(root_.get(), other.root_.get());
}

template <typename T, template <typename> class Node>
bool BSTBase<T, Node>::operator!=(const BSTBase<T, Node>& other) const {
    return !(*this == other);
}

// Insertion and deletion functions

template <typename T, template <typename> class Node>
void BSTBase<T, Node>::insert(const T& key) {  // O(h)
    insertAndReturnNewNode(key);
}

template <typename T, template <typename> class Node>
void BSTBase<T, Node>::erase(const T& key) {  // O(h)
    Node<T>* toDelete = findNode(key);
    if (toDelete != nullptr)
        erase(toDelete);
}

template <typename T, template <typename> class Node>
void BSTBase<T, Node>::erase(iterator& it) {
    if (it.currentNode_ != nullptr) {
        erase(it.currentNode_);
        invalidateIterator(it);
    }
}

template <typename T, template <typename> class Node>
void BSTBase<T, Node>::erase(iterator&& it) {
    if (it.currentNode_ != nullptr) {
        erase(it.currentNode_);
        invalidateIterator(it);
    }
}

// public Utility

template <typename T, template <typename> class Node>
void BSTBase<T, Node>::clear() {
    root_ = nullptr;
}

// Traversals

template <typename T, template <typename> class Node>
template <class Container>
Container BSTBase<T, Node>::inorder() const {
    Container result(computeSize());
    size_t currentIndex = 0;
    subtreeInorder(root_.get(), result, currentIndex);
    return result;
}

template <typename T, template <typename> class Node>
template <class Container>
Container BSTBase<T, Node>::preorder() const {
    Container result(computeSize());
    size_t currentIndex = 0;
    subtreePreorder(root_.get(), result, currentIndex);
    return result;
}

template <typename T, template <typename> class Node>
template <class Container>
Container BSTBase<T, Node>::postorder() const {
    Container result(computeSize());
    size_t currentIndex = 0;
    subtreePostorder(root_.get(), result, currentIndex);
    return result;
}

template <typename T, template <typename> class Node>
bool BSTBase<T, Node>::isEmpty() const {
    return root_ == nullptr;
}

template <typename T, template <typename> class Node>
size_t BSTBase<T, Node>::computeHeight() const {
    return subtreeHeight(root_.get());
}

template <typename T, template <typename> class Node>
size_t BSTBase<T, Node>::computeSize() const {
    return subtreeSize(root_.get());
}

template <typename T, template <typename> class Node>
typename BSTBase<T, Node>::iterator BSTBase<T, Node>::find(const T& key) const {
    return iterator(findNode(key));
}

// Min / Max functions

template <typename T, template <typename> class Node>
typename BSTBase<T, Node>::iterator BSTBase<T, Node>::min() const {  // O(h)
    return iterator(subtreeMin(root_.get()));
}

template <typename T, template <typename> class Node>
typename BSTBase<T, Node>::iterator BSTBase<T, Node>::max() const {  // O(h)
    return iterator(subtreeMax(root_.get()));
}

template <typename T, template <typename> class Node>
T BSTBase<T, Node>::minKey() const {
    return subtreeMin(root_.get())->key;
}

template <typename T, template <typename> class Node>
T BSTBase<T, Node>::maxKey() const {
    return subtreeMax(root_.get())->key;
}

template <typename T, template <typename> class Node>
T BSTBase<T, Node>::extractMin() {
    iterator minIt = min();
    T key = minIt.key();
    erase(minIt);
    return key;
}

template <typename T, template <typename> class Node>
T BSTBase<T, Node>::extractMax() {
    iterator maxIt = max();
    T key = maxIt.key();
    erase(maxIt);
    return key;
}

template <typename T, template <typename> class Node>
typename BSTBase<T, Node>::iterator BSTBase<T, Node>::root() const {
    return iterator(root_.get());
}

// Protected utility functions

template <typename T, template <typename> class Node>
std::unique_ptr<Node<T>>& BSTBase<T, Node>::getUnique(Node<T>* node) {  // Can't handle nullptr
    if (node->parent == nullptr)
        return root_;
    else if (node == node->parent->left.get())
        return node->parent->left;
    else
        return node->parent->right;
}

template <typename T, template <typename> class Node>
Node<T>* BSTBase<T, Node>::getPtr(iterator it) {
    return it.currentNode_;
}

template <typename T, template <typename> class Node>
void BSTBase<T, Node>::invalidateIterator(iterator& it) {
    it.currentNode_ = nullptr;
}

template <typename T, template <typename> class Node>
Node<T>* BSTBase<T, Node>::insertAndReturnNewNode(const T& key) {
    Node<T>* it = root_.get();
    Node<T>* itParent = nullptr;

    while (it != nullptr) {
        itParent = it;

        if (it->key > key)
            it = it->left.get();
        else
            it = it->right.get();
    }

    if (itParent == nullptr) {
        root_ = std::make_unique<Node<T>>(key);
        return root_.get();
    } else if (itParent->key > key) {
        itParent->left = std::make_unique<Node<T>>(key, itParent);
        return itParent->left.get();
    } else {
        itParent->right = std::make_unique<Node<T>>(key, itParent);
        return itParent->right.get();
    }
}

template <typename T, template <typename> class Node>
void BSTBase<T, Node>::erase(Node<T>* toDelete) {
    if (toDelete->left == nullptr) {
        transplant(toDelete, toDelete->right);
    } else if (toDelete->right == nullptr) {
        transplant(toDelete, toDelete->left);
    } else {
        Node<T>* replacement = subtreeMin(toDelete->right.get());

        if (replacement->parent != toDelete) {
            Node<T>* rParent = replacement->parent;

            std::unique_ptr<Node<T>> tmp = std::move(getUnique(replacement));  // Save replacement
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

template <typename T, template <typename> class Node>
Node<T>* BSTBase<T, Node>::findReplacement(Node<T>* node) {
    if (node->left == nullptr && node->right == nullptr)
        return nullptr;
    else if (node->left == nullptr)
        return node->right.get();
    else if (node->right == nullptr)
        return node->left.get();
    else
        return subtreeMin(node->right.get());
}

// Rotations

template <typename T, template <typename> class Node>
void BSTBase<T, Node>::rotateLeft(Node<T>* node) {
    std::unique_ptr<Node<T>> rightChild = std::move(node->right);
    Node<T>* rightChildPtr = rightChild.get();

    if (rightChild->left != nullptr)
        rightChild->left->parent = node;
    node->right = std::move(rightChild->left);

    bool isLeftChild = false;
    if (node->parent != nullptr && node == node->parent->left.get())
        isLeftChild = true;

    std::unique_ptr<Node<T>> nodeTmp = std::move(getUnique(node));
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

template <typename T, template <typename> class Node>
void BSTBase<T, Node>::rotateRight(Node<T>* node) {
    std::unique_ptr<Node<T>> leftChild = std::move(node->left);
    Node<T>* leftChildPtr = leftChild.get();

    if (leftChild->right != nullptr)
        leftChild->right->parent = node;
    node->left = std::move(leftChild->right);

    bool isLeftChild = false;
    if (node->parent != nullptr && node == node->parent->left.get())
        isLeftChild = true;

    std::unique_ptr<Node<T>> nodeTmp = std::move(getUnique(node));
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

template <typename T, template <typename> class Node>
Node<T>* BSTBase<T, Node>::findNode(const T& key) const {  // O(h)
    Node<T>* it = root_.get();

    while (it != nullptr && it->key != key) {
        if (it->key > key)
            it = it->left.get();
        else
            it = it->right.get();
    }

    return it;
}

template <typename T, template <typename> class Node>
bool BSTBase<T, Node>::subtreeEqual(Node<T>* node, Node<T>* otherNode) const {
    if (node == nullptr && otherNode == nullptr)
        return true;
    else if (node == nullptr && otherNode != nullptr || node != nullptr && otherNode == nullptr)
        return false;
    else if (node->key != otherNode->key)
        return false;
    else
        return subtreeEqual(node->left.get(), otherNode->left.get()) && subtreeEqual(node->right.get(), otherNode->right.get());
}

template <typename T, template <typename> class Node>
void BSTBase<T, Node>::transplant(Node<T>* toDelete, Node<T>* replacement) {  // This does not work, and I still need to find out why
    std::unique_ptr<Node<T>> replacementUnique = (replacement != nullptr) ? std::move(getUnique(replacement)) : nullptr;
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

template <typename T, template <typename> class Node>
void BSTBase<T, Node>::transplant(Node<T>* toDelete, std::unique_ptr<Node<T>>& replacement) {  // O(1)
    if (replacement != nullptr)
        replacement->parent = toDelete->parent;

    if (toDelete->parent == nullptr)
        root_ = std::move(replacement);
    else if (toDelete == toDelete->parent->left.get())
        toDelete->parent->left = std::move(replacement);
    else
        toDelete->parent->right = std::move(replacement);
}

template <typename T, template <typename> class Node>
Node<T>* BSTBase<T, Node>::subtreeMin(Node<T>* subTreeRoot) const {  // O(h)
    Node<T>* it = subTreeRoot;

    while (it != nullptr && it->left != nullptr)
        it = it->left.get();

    return it;
}

template <typename T, template <typename> class Node>
Node<T>* BSTBase<T, Node>::subtreeMax(Node<T>* subTreeRoot) const {  // O(h)
    Node<T>* it = subTreeRoot;

    while (it != nullptr && it->right != nullptr)
        it = it->right.get();

    return it;
}

// private utility

template <typename T, template <typename> class Node>
size_t BSTBase<T, Node>::subtreeHeight(Node<T>* subtreeRoot) const {
    if (subtreeRoot == nullptr)
        return 0;

    return std::max(subtreeHeight(subtreeRoot->right.get()), subtreeHeight(subtreeRoot->left.get())) + 1;
}

template <typename T, template <typename> class Node>
size_t BSTBase<T, Node>::subtreeSize(Node<T>* subtreeRoot) const {
    if (subtreeRoot == nullptr)
        return 0;

    return subtreeSize(subtreeRoot->right.get()) + subtreeSize(subtreeRoot->left.get()) + 1;
}

template <typename T, template <typename> class Node>
template <class Container>
void BSTBase<T, Node>::subtreeInorder(Node<T>* subtreeRoot, Container& trav, size_t& currentIndex) const {
    if (subtreeRoot != nullptr) {
        subtreeInorder(subtreeRoot->left.get(), trav, currentIndex);
        trav[currentIndex] = subtreeRoot->key;
        ++currentIndex;
        subtreeInorder(subtreeRoot->right.get(), trav, currentIndex);
    }
}

template <typename T, template <typename> class Node>
template <class Container>
void BSTBase<T, Node>::subtreePreorder(Node<T>* subtreeRoot, Container& trav, size_t& currentIndex) const {
    if (subtreeRoot != nullptr) {
        trav[currentIndex] = subtreeRoot->key;
        ++currentIndex;
        subtreePreorder(subtreeRoot->left.get(), trav, currentIndex);
        subtreePreorder(subtreeRoot->right.get(), trav, currentIndex);
    }
}

template <typename T, template <typename> class Node>
template <class Container>
void BSTBase<T, Node>::subtreePostorder(Node<T>* subtreeRoot, Container& trav, size_t& currentIndex) const {
    if (subtreeRoot != nullptr) {
        subtreePostorder(subtreeRoot->left.get(), trav, currentIndex);
        subtreePostorder(subtreeRoot->right.get(), trav, currentIndex);
        trav[currentIndex] = subtreeRoot->key;
        ++currentIndex;
    }
}