#pragma once

#include "TreeNode.h"

template <typename T>
class BinarySearchTree;

template <typename T>
class TreeTraversal {
   public:
    TreeTraversal() {}
    virtual TreeNode<T>* startNode(const BinarySearchTree<T>& tree) const = 0;
    virtual TreeNode<T>* lastNode(const BinarySearchTree<T>& tree) const = 0;

    virtual TreeNode<T>* getSuccessor(TreeNode<T>* currentNode, const BinarySearchTree<T>& tree) const = 0;
};

#include "BinarySearchTree.h"