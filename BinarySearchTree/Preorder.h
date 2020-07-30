#pragma once

#include "TreeTraversal.h"

template <typename T>
class Preorder : public TreeTraversal<T> {
   public:
    Preorder() : TreeTraversal<T>() {}

    TreeNode<T>* startNode(const BinarySearchTree<T>& tree) const override;
    TreeNode<T>* lastNode(const BinarySearchTree<T>& tree) const override;

    TreeNode<T>* getSuccessor(TreeNode<T>* currentNode, const BinarySearchTree<T>& tree) const override;
};

template <typename T>
TreeNode<T>* Preorder<T>::startNode(const BinarySearchTree<T>& tree) const {  // O(1)
    return tree.root_.get();
}

template <typename T>
TreeNode<T>* Preorder<T>::lastNode(const BinarySearchTree<T>& tree) const {  // O(h)
    return tree.maxSubtree(tree.root_.get());
}

template <typename T>
TreeNode<T>* Preorder<T>::getSuccessor(TreeNode<T>* currentNode, const BinarySearchTree<T>& tree) const {  // O(h)
    if (currentNode->left != nullptr)
        return currentNode->left.get();
    else if (currentNode->right != nullptr)
        return currentNode->right.get();

    TreeNode<T>* it = currentNode->parent;

    while (it != nullptr) {
        if (currentNode->key < it->key && it->right != nullptr)
            break;
        it = it->parent;
    }

    return it != nullptr ? it->right.get() : nullptr;
}