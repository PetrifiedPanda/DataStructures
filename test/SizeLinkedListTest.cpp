#include <gtest/gtest.h>

#include "LinkedList/SizeLinkedList.h"

struct SizeLinkedListTests : public testing::Test {
    SizeLinkedList<int> lst;

    virtual void SetUp() override {
        lst = SizeLinkedList<int>({1, 2, 3, 4, 5});
    }

    virtual void TearDown() override {
    }
};

TEST_F(SizeLinkedListTests, BasicUsage) {
    // Don't use predeclared Stuff
    SizeLinkedList<double> myLst({0.1, 0.2, 0.3, 0.4, 0.5, 0.6});

    EXPECT_EQ(6, myLst.size());
}

TEST_F(SizeLinkedListTests, Constructor) {
    SizeLinkedList<int> lstCpy(lst);
    // Check equality
    auto lstIt = lst.begin();
    auto cpyIt = lstCpy.begin();
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(*lstIt, *cpyIt);
        ++lstIt;
        ++cpyIt;
    }

    SizeLinkedList<int> lstMove(std::move(lst));
    // Check pointer equality
    EXPECT_TRUE(lst.isEmpty());
}

TEST_F(SizeLinkedListTests, Append) {
    lst.clear();

    lst.append(1);
    EXPECT_EQ(1, lst.size());
    lst.append(2);
    EXPECT_EQ(2, lst.size());
    lst.append(3);
    EXPECT_EQ(3, lst.size());
    lst.append(4);
    EXPECT_EQ(4, lst.size());
    lst.append(5);
    EXPECT_EQ(5, lst.size());

    int count = 1;
    for (const auto& item : lst) {
        EXPECT_EQ(count, item);
        ++count;
    }
}

TEST_F(SizeLinkedListTests, Insertion) {
    lst.clear();

    lst.insert(1, lst.begin());
    EXPECT_EQ(1, lst.size());
    lst.insert(4, ++lst.begin());
    EXPECT_EQ(2, lst.size());
    lst.insert(2, ++lst.begin());
    EXPECT_EQ(3, lst.size());
    lst.insert(3, ++++lst.begin());
    EXPECT_EQ(4, lst.size());
    lst.insert(5, ++++++++lst.begin());
    EXPECT_EQ(5, lst.size());

    int count = 1;
    for (const auto& item : lst) {
        EXPECT_EQ(count, item);
        ++count;
    }
}

TEST_F(SizeLinkedListTests, Deletion) {
    EXPECT_TRUE(false) << "Tests not implemented";
}

TEST_F(SizeLinkedListTests, RangeDeletion) {
    EXPECT_TRUE(false) << "Tests not implemented";
}