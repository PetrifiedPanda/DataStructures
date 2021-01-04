#pragma once

#include "BSTBase.h"
#include "TreeNode.h"

template <typename T>
class BSTNode {
   public:
    BasicTreeNode(BSTNode, T);
};

template <typename T>
using BinarySearchTree = BSTBase<T, BSTNode>;