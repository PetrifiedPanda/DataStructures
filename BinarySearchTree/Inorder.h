#pragma once

#include "TreeTraversal.h"

template <typename T>
class Inorder : public TreeTraversal<T> {
   public:
    Inorder() : TreeTraversal<T>() {}

    TreeNode<T>* startNode(const BinarySearchTree<T>& tree) const override;
    TreeNode<T>* lastNode(const BinarySearchTree<T>& tree) const override;

    TreeNode<T>* getSuccessor(TreeNode<T>* currentNode, const BinarySearchTree<T>& tree) const override;
};

template <typename T>
TreeNode<T>* Inorder<T>::startNode(const BinarySearchTree<T>& tree) const {  // O(h)
    return tree.subtreeMin(tree.root_.get());
}

template <typename T>
TreeNode<T>* Inorder<T>::lastNode(const BinarySearchTree<T>& tree) const {  // O(h)
    return tree.subtreeMax(tree.root_.get());
}

template <typename T>
TreeNode<T>* Inorder<T>::getSuccessor(TreeNode<T>* currentNode, const BinarySearchTree<T>& tree) const {  // O(h)
    if (currentNode->right != nullptr) {
        return tree.subtreeMin(currentNode->right.get());
    } else {
        TreeNode<T>* rightMost = tree.subtreeMax(tree.root_.get());

        if (rightMost == currentNode)
            return nullptr;
        else {
            TreeNode<T>* it = currentNode;
            while (it != tree.root_.get() && it == it->parent->right.get())
                it = it->parent;

            return it->parent;
        }
    }
}