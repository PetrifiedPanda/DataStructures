#pragma once

#include "TreeTraversal.h"

template <typename T>
class Postorder : public TreeTraversal<T> {
   public:
    Postorder() : TreeTraversal<T>() {}

    TreeNode<T>* startNode(const BinarySearchTree<T>& tree) const override;
    TreeNode<T>* lastNode(const BinarySearchTree<T>& tree) const override;

    TreeNode<T>* getSuccessor(TreeNode<T>* currentNode, const BinarySearchTree<T>& tree) const override;
    TreeNode<T>* findPostorderInRightSubTree(TreeNode<T>* currentNode) const;
};

template <typename T>
TreeNode<T>* Postorder<T>::startNode(const BinarySearchTree<T>& tree) const {  // O(h)
    return tree.minSubtree(tree.root_.get());
}

template <typename T>
TreeNode<T>* Postorder<T>::lastNode(const BinarySearchTree<T>& tree) const {  // O(1)
    return tree.root_.get();
}

template <typename T>
TreeNode<T>* Postorder<T>::getSuccessor(TreeNode<T>* currentNode, const BinarySearchTree<T>& tree) const {  // O(h) and not working
    if (currentNode == tree.root_.get())
        return nullptr;

    TreeNode<T>* parent = currentNode->parent;
    if (parent->right == nullptr || currentNode == parent->right.get())
        return parent;

    return findPostorderInRightSubTree(parent->right.get());
}

template <typename T>
TreeNode<T>* Postorder<T>::findPostorderInRightSubTree(TreeNode<T>* currentNode) const {
    if (currentNode == nullptr)
        return nullptr;

    while (currentNode != nullptr) {
        TreeNode<T>* p = currentNode;
        currentNode = currentNode->left.get();

        if (currentNode == nullptr) {
            if (p->right == nullptr)
                return p;
            else
                return findPostorderInRightSubTree(p->right.get());
        }
    }
    return nullptr;
}