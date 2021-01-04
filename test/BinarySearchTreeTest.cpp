#include <gtest/gtest.h>

#include <ctime>
#include <random>

#include "BinarySearchTree/BinarySearchTree.h"

struct BinarySearchTreeTests : public testing::Test {
    BinarySearchTree<int> tree;

    virtual void SetUp() override {
        tree = BinarySearchTree<int>();

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

TEST_F(BinarySearchTreeTests, BasicUsage) {
    BinarySearchTree<double> myTree;

    myTree.insert(5);
    myTree.insert(2.5);
    myTree.insert(7.5);

    EXPECT_EQ(5, myTree.root().key());
    EXPECT_EQ(2.5, myTree.root().left().key());
    EXPECT_EQ(7.5, myTree.root().right().key());

    auto it = myTree.find(7.5);
    EXPECT_EQ(5, myTree.root().key());

    myTree.erase(it);
    EXPECT_EQ(5, myTree.root().key());
    EXPECT_EQ(2.5, myTree.root().left().key());

    EXPECT_FALSE(myTree.root().right().isValid());

    myTree.erase(5);
    EXPECT_EQ(2.5, myTree.root().key());

    EXPECT_FALSE(myTree.root().left().isValid());
    EXPECT_FALSE(myTree.root().right().isValid());

    BinarySearchTree<double> myTree2;
    myTree2.insert(3);
    myTree2.insert(4);
    EXPECT_FALSE(myTree == myTree2);
}

TEST_F(BinarySearchTreeTests, Constructors) {
    BinarySearchTree<int> treeCpy(tree);
    ASSERT_EQ(tree, treeCpy);  // if the copy is not equal we have nothing to compare the move result to

    BinarySearchTree<int> treeMove(std::move(tree));
    EXPECT_EQ(treeCpy, treeMove);
}

TEST_F(BinarySearchTreeTests, Insertion) {
    auto it = tree.root();
    EXPECT_EQ(40, it.key());

    auto leftIt = it.left();
    EXPECT_EQ(20, leftIt.key());
    auto rightIt = it.right();
    EXPECT_EQ(60, rightIt.key());

    leftIt = leftIt.left();
    EXPECT_EQ(10, leftIt.key());
    leftIt = leftIt.parent();
    EXPECT_EQ(20, leftIt.key());
    leftIt = leftIt.right();
    EXPECT_EQ(30, leftIt.key());

    rightIt = rightIt.left();
    EXPECT_EQ(50, rightIt.key());
    rightIt = rightIt.parent();
    EXPECT_EQ(60, rightIt.key());
    rightIt = rightIt.right();
    EXPECT_EQ(70, rightIt.key());
}

TEST_F(BinarySearchTreeTests, Deletion) {
    tree.erase(tree.root());

    auto it = tree.root();
    EXPECT_EQ(50, it.key());

    auto leftIt = it.left();
    EXPECT_EQ(20, leftIt.key());
    auto rightIt = it.right();
    EXPECT_EQ(60, rightIt.key());

    auto nullNode = rightIt.left();
    EXPECT_FALSE(nullNode.isValid());
    rightIt = rightIt.right();
    EXPECT_EQ(70, rightIt.key());

    auto rootIt = tree.root();
    tree.erase(rootIt);
    EXPECT_FALSE(rootIt.isValid());
}

TEST_F(BinarySearchTreeTests, Inorder) {
    auto vec = tree.inorder<std::vector<int>>();
    std::vector<int> expected = {10, 20, 30, 40, 50, 60, 70};
    EXPECT_EQ(expected, vec);
}

TEST_F(BinarySearchTreeTests, Preorder) {
    auto vec = tree.preorder<std::vector<int>>();
    std::vector<int> expected = {40, 20, 10, 30, 60, 50, 70};
    EXPECT_EQ(expected, vec);
}

TEST_F(BinarySearchTreeTests, Postorder) {
    auto vec = tree.postorder<std::vector<int>>();
    std::vector<int> expected = {10, 30, 20, 50, 70, 60, 40};
    EXPECT_EQ(expected, vec);
}

struct BinarySearchTreeRandomTests : public testing::Test {
    int samples = 1000;
    std::default_random_engine engine = std::default_random_engine(time(nullptr));

    std::uniform_int_distribution<int> dist = std::uniform_int_distribution<int>(0, 1000);

    BinarySearchTree<int> tree;

    virtual void SetUp() override {
        tree = BinarySearchTree<int>();
        for (int i = 0; i < samples; ++i)
            tree.insert(dist(engine));
    }

    virtual void TearDown() override {
    }
};

TEST_F(BinarySearchTreeRandomTests, ExtractMax) {
    int lastMax = std::numeric_limits<int>::max();
    while (!tree.isEmpty()) {
        int max = tree.extractMax();
        EXPECT_LE(max, lastMax);
        lastMax = max;
    }
}

TEST_F(BinarySearchTreeRandomTests, ExtractMin) {
    int lastMin = std::numeric_limits<int>::min();
    while (!tree.isEmpty()) {
        int min = tree.extractMin();
        EXPECT_GE(min, lastMin);
        lastMin = min;
    }
}