#include <gtest/gtest.h>

#include "BinarySearchTree/SplayTree.h"

struct SplayTreeTests : public testing::Test {
    SplayTree<int> tree;

    virtual void SetUp() override {
        tree = SplayTree<int>();

        tree.insert(40);
        tree.insert(20);
        tree.insert(60);
        tree.insert(10);
        tree.insert(30);
        tree.insert(50);
        tree.insert(70);
    }

    virtual void TearDown() override {
    }
};

TEST_F(SplayTreeTests, BasicUsage) {
    SplayTree<double> myTree;

    myTree.insert(5);
    myTree.insert(2.5);
    myTree.insert(7.5);

    EXPECT_EQ(7.5, myTree.root().key());

    auto it = myTree.find(5);
    EXPECT_EQ(5, myTree.root().key());

    myTree.erase(it.getLeft());
    EXPECT_EQ(5, myTree.root().key());
    EXPECT_EQ(7.5, myTree.root().right().key());

    EXPECT_FALSE(it.left().isValid());

    myTree.erase(5);
    EXPECT_EQ(7.5, myTree.root().key());

    EXPECT_FALSE(myTree.root().left().isValid());
    EXPECT_FALSE(myTree.root().right().isValid());

    SplayTree<double> myTree2;
    myTree2.insert(3);
    myTree2.insert(4);
    EXPECT_FALSE(myTree == myTree2);
}

TEST_F(SplayTreeTests, Constructor) {
    SplayTree<int> treeCpy(tree);
    ASSERT_EQ(tree, treeCpy);

    SplayTree<int> treeMove(std::move(tree));
    EXPECT_EQ(treeCpy, treeMove);
}

TEST_F(SplayTreeTests, Insertion) {
    tree.clear();
    tree.insert(40);
    EXPECT_EQ(40, tree.root().key());
    tree.insert(20);
    EXPECT_EQ(20, tree.root().key());
    EXPECT_EQ(40, tree.root().right().key());
    tree.insert(60);
    EXPECT_EQ(60, tree.root().key());
    EXPECT_EQ(40, tree.root().left().key());
    EXPECT_EQ(20, tree.root().left().left().key());
    tree.insert(10);
    EXPECT_EQ(10, tree.root().key());
    EXPECT_EQ(60, tree.root().right().key());
    EXPECT_EQ(20, tree.root().right().left().key());
    EXPECT_EQ(40, tree.root().right().left().right().key());
    tree.insert(30);
    EXPECT_EQ(30, tree.root().key());
    EXPECT_EQ(10, tree.root().left().key());
    EXPECT_EQ(20, tree.root().left().right().key());
    EXPECT_EQ(60, tree.root().right().key());
    EXPECT_EQ(40, tree.root().right().left().key());
    tree.insert(50);
    EXPECT_EQ(50, tree.root().key());
    EXPECT_EQ(60, tree.root().right().key());
    EXPECT_EQ(30, tree.root().left().key());
    EXPECT_EQ(40, tree.root().left().right().key());
    EXPECT_EQ(10, tree.root().left().left().key());
    EXPECT_EQ(20, tree.root().left().left().right().key());
    tree.insert(70);
    EXPECT_EQ(70, tree.root().key());
    EXPECT_EQ(60, tree.root().left().key());
    EXPECT_EQ(50, tree.root().left().left().key());
    EXPECT_EQ(30, tree.root().left().left().left().key());
    EXPECT_EQ(40, tree.root().left().left().left().right().key());
    EXPECT_EQ(10, tree.root().left().left().left().left().key());
    EXPECT_EQ(20, tree.root().left().left().left().left().right().key());
}

TEST_F(SplayTreeTests, Deletion) {
    tree.erase(tree.root().left());
    EXPECT_EQ(50, tree.root().key());
    EXPECT_EQ(70, tree.root().right().key());
    EXPECT_EQ(30, tree.root().left().key());
    EXPECT_EQ(40, tree.root().left().right().key());
    EXPECT_EQ(10, tree.root().left().left().key());
    EXPECT_EQ(20, tree.root().left().left().right().key());

    EXPECT_EQ(50, tree.root().right().parent().key());
    EXPECT_EQ(50, tree.root().left().parent().key());

    tree.erase(tree.root().left());
    EXPECT_EQ(20, tree.root().key());
    EXPECT_EQ(10, tree.root().left().key());
    EXPECT_EQ(50, tree.root().right().key());
    EXPECT_EQ(70, tree.root().right().right().key());
    EXPECT_EQ(40, tree.root().right().left().key());

    EXPECT_EQ(20, tree.root().right().parent().key());
    EXPECT_EQ(20, tree.root().left().parent().key());

    tree.erase(tree.root().right());
    EXPECT_EQ(40, tree.root().key());
    EXPECT_EQ(70, tree.root().right().key());
    EXPECT_EQ(20, tree.root().left().key());
    EXPECT_EQ(10, tree.root().left().left().key());

    EXPECT_EQ(40, tree.root().right().parent().key());
    EXPECT_EQ(40, tree.root().left().parent().key());

    tree.erase(tree.root().left().left());
    EXPECT_EQ(20, tree.root().key());
    EXPECT_EQ(40, tree.root().right().key());
    EXPECT_EQ(70, tree.root().right().right().key());

    EXPECT_FALSE(tree.root().left().isValid());

    EXPECT_EQ(20, tree.root().right().parent().key());

    tree.erase(tree.root());
    EXPECT_EQ(40, tree.root().key());
    EXPECT_EQ(70, tree.root().right().key());

    EXPECT_FALSE(tree.root().left().isValid());

    EXPECT_EQ(40, tree.root().right().parent().key());

    tree.find(70);
    tree.erase(tree.root());
    EXPECT_EQ(40, tree.root().key());

    EXPECT_FALSE(tree.root().left().isValid());
    EXPECT_FALSE(tree.root().right().isValid());

    tree.erase(tree.root());
    EXPECT_FALSE(tree.root().isValid());
    EXPECT_TRUE(tree.isEmpty());
}