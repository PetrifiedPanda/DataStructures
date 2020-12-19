#include <random>
#include <ctime>

#include <gtest/gtest.h>

#include "BinarySearchTree/RedBlackTree.h"

struct RedBlackTreeTests : public testing::Test {
    RedBlackTree<int> tree;

    virtual void SetUp() override {
        tree = RedBlackTree<int>();

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

TEST_F(RedBlackTreeTests, BasicUsage) {
    RedBlackTree<double> myTree;

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

    RedBlackTree<double> myTree2;
    myTree2.insert(3);
    myTree2.insert(4);
    EXPECT_FALSE(myTree == myTree2);
}

TEST_F(RedBlackTreeTests, Constructors) {
    RedBlackTree<int> treeCpy(tree);
    ASSERT_EQ(tree, treeCpy);  // if the copy is not equal we have nothing to compare the move result to

    RedBlackTree<int> treeMove(std::move(tree));
    EXPECT_EQ(treeCpy, treeMove);
}

TEST_F(RedBlackTreeTests, Insertion) {
    tree.clear();
    tree.insert(40);
    EXPECT_EQ(40, tree.root().key());

    tree.insert(20);
    EXPECT_EQ(40, tree.root().key());
    EXPECT_EQ(20, tree.root().left().key());

    tree.insert(10);
    EXPECT_EQ(20, tree.root().key());
    EXPECT_EQ(10, tree.root().left().key());
    EXPECT_EQ(40, tree.root().right().key());

    tree.insert(50);
    EXPECT_EQ(20, tree.root().key());
    EXPECT_EQ(10, tree.root().left().key());
    EXPECT_EQ(40, tree.root().right().key());
    EXPECT_EQ(50, tree.root().right().right().key());

    tree.insert(60);
    EXPECT_EQ(20, tree.root().key());
    EXPECT_EQ(10, tree.root().left().key());
    EXPECT_EQ(50, tree.root().right().key());
    EXPECT_EQ(40, tree.root().right().left().key());
    EXPECT_EQ(60, tree.root().right().right().key());

    tree.insert(70);
    EXPECT_EQ(20, tree.root().key());
    EXPECT_EQ(10, tree.root().left().key());
    EXPECT_EQ(50, tree.root().right().key());
    EXPECT_EQ(40, tree.root().right().left().key());
    EXPECT_EQ(60, tree.root().right().right().key());
    EXPECT_EQ(70, tree.root().right().right().right().key());

    tree.insert(30);
    EXPECT_EQ(20, tree.root().key());
    EXPECT_EQ(10, tree.root().left().key());
    EXPECT_EQ(50, tree.root().right().key());
    EXPECT_EQ(40, tree.root().right().left().key());
    EXPECT_EQ(30, tree.root().right().left().left().key());
    EXPECT_EQ(60, tree.root().right().right().key());
    EXPECT_EQ(70, tree.root().right().right().right().key());
}

TEST_F(RedBlackTreeTests, Deletion) {
    tree.erase(tree.root());

    EXPECT_EQ(50, tree.root().key());
    EXPECT_EQ(20, tree.root().left().key());
    EXPECT_EQ(10, tree.root().left().left().key());
    EXPECT_EQ(30, tree.root().left().right().key());
    EXPECT_EQ(60, tree.root().right().key());
    EXPECT_EQ(70, tree.root().right().right().key());

    EXPECT_FALSE(tree.root().right().left().isValid());

    tree.erase(tree.root().right());
    EXPECT_EQ(50, tree.root().key());
    EXPECT_EQ(20, tree.root().left().key());
    EXPECT_EQ(10, tree.root().left().left().key());
    EXPECT_EQ(30, tree.root().left().right().key());
    EXPECT_EQ(70, tree.root().right().key());

    tree.erase(tree.root().right());
    EXPECT_EQ(20, tree.root().key());
    EXPECT_EQ(10, tree.root().left().key());
    EXPECT_EQ(50, tree.root().right().key());
    EXPECT_EQ(30, tree.root().right().left().key());

    tree.erase(tree.root().left());
    EXPECT_EQ(30, tree.root().key());
    EXPECT_EQ(20, tree.root().left().key());
    EXPECT_EQ(50, tree.root().right().key());

    tree.erase(tree.root());
    EXPECT_EQ(50, tree.root().key());
    EXPECT_EQ(20, tree.root().left().key());

    EXPECT_FALSE(tree.root().right().isValid());

    tree.erase(tree.root().left());
    EXPECT_EQ(50, tree.root().key());

    EXPECT_FALSE(tree.root().left().isValid());
    EXPECT_FALSE(tree.root().right().isValid());

    tree.erase(tree.root());
    EXPECT_FALSE(tree.root().isValid());
}

TEST_F(RedBlackTreeTests, Inorder) {
    auto vec = tree.inorder<std::vector<int>>();
    std::vector<int> expected = {10, 20, 30, 40, 50, 60, 70};
    EXPECT_EQ(expected, vec);
}

TEST_F(RedBlackTreeTests, Preorder) {
    auto vec = tree.preorder<std::vector<int>>();
    std::vector<int> expected = {40, 20, 10, 30, 60, 50, 70};
    EXPECT_EQ(expected, vec);
}

TEST_F(RedBlackTreeTests, Postorder) {
    auto vec = tree.postorder<std::vector<int>>();
    std::vector<int> expected = {10, 30, 20, 50, 70, 60, 40};
    EXPECT_EQ(expected, vec);
}

struct RedBlackTreeRandomTests : public testing::Test {
    int samples = 1000;
    std::default_random_engine engine = std::default_random_engine(time(nullptr));

    std::uniform_int_distribution<int> dist = std::uniform_int_distribution<int>(0, 1000);

    RedBlackTree<int> tree;

    virtual void SetUp() override {
        tree = RedBlackTree<int>();
        for (int i = 0; i < samples; ++i)
            tree.insert(dist(engine));
    }

    virtual void TearDown() override {
    }
};

TEST_F(RedBlackTreeRandomTests, InsertTests) {
    tree.clear();

    for (int i = 0; i < samples; ++i) {
        tree.insert(dist(engine));
        EXPECT_LE(tree.computeHeight(), 2 * log2(tree.computeSize() + 1) + 1);
    }
}

// Because these tests work, deletion of nodes with zero to one children should work
TEST_F(RedBlackTreeRandomTests, ExtractMax) {
    int lastMax = std::numeric_limits<int>::max();
    while (!tree.isEmpty()) {
        int max = tree.extractMax();
        EXPECT_LE(max, lastMax);
        lastMax = max;
        EXPECT_LE(tree.computeHeight(), 2 * log2(tree.computeSize() + 1) + 1);
    }
}

TEST_F(RedBlackTreeRandomTests, ExtractMin) {
    int lastMin = std::numeric_limits<int>::min();
    while (!tree.isEmpty()) {
        int min = tree.extractMin();
        EXPECT_GE(min, lastMin);
        lastMin = min;
        EXPECT_LE(tree.computeHeight(), 2 * log2(tree.computeSize() + 1) + 1);
    }
}

// The height of a Red-Black Tree is guaranteed to be 2 * log2(n + 1) + 1 (n is the number of elements)
TEST_F(RedBlackTreeRandomTests, RootDeletion) {
    int size = tree.computeSize();
    for (int i = 0; i < samples; ++i) {
        tree.erase(tree.root());
        EXPECT_LE(tree.computeHeight(), 2 * log2(size) + 1);
        --size;
    }
}

typename RedBlackTree<int>::iterator getRandomNode(const RedBlackTree<int>& tree) {
    std::default_random_engine engine(time(nullptr));
    std::uniform_int_distribution<int> dist(0, 100);

    auto it = tree.root();

    while (true) {
        if (dist(engine) < 50)
            return it;

        if (it.hasLeftChild() && it.hasRightChild()) {
            if (dist(engine) < 50)
                it.left();
            else
                it.right();
        } else if (it.hasLeftChild())
            it.left();
        else if (it.hasRightChild())
            it.right();
        else
            return it;

        if (dist(engine) < 50)
            return it;
    }
}

TEST_F(RedBlackTreeRandomTests, RandomNodeDeletion) {
    int size = tree.computeSize();
    for (int i = 0; i < samples; ++i) {
        tree.erase(getRandomNode(tree));
        EXPECT_LE(tree.computeHeight(), 2 * log2(size) + 1);
        --size;
    }
}