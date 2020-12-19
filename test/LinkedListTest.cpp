#include <gtest/gtest.h>

#include "LinkedList/LinkedList.h"

struct LinkedListTests : public testing::Test {
    LinkedList<int> lst;

    virtual void SetUp() override {
        lst = LinkedList<int>({1, 2, 3, 4, 5});
    }

    virtual void TearDown() override {
    }
};

TEST_F(LinkedListTests, BasicUsage) {
    // Don't use predeclared Stuff
}

TEST_F(LinkedListTests, Constructor) {
    LinkedList<int> lstCpy(lst);
    // Check equality
    auto lstIt = lst.begin();
    auto cpyIt = lstCpy.begin();
    for (int i = 0; i < lst.computeSize(); ++i) {
        ASSERT_EQ(*lstIt, *cpyIt);
        ++lstIt;
        ++cpyIt;
    }

    LinkedList<int> lstMove(std::move(lst));
    auto moveIt = lstMove.begin();
    for (int i = 0; i < lst.computeSize(); ++i) {
        EXPECT_EQ(*cpyIt, *moveIt);
        ++cpyIt;
        ++moveIt;
    }
    // Check pointer equality
    EXPECT_TRUE(lst.isEmpty());
}

TEST_F(LinkedListTests, Append) {
    lst.clear();

    lst.append(1);
    lst.append(2);
    lst.append(3);
    lst.append(4);
    lst.append(5);

    int count = 1;
    for (const auto& item : lst) {
        EXPECT_EQ(count, item);
        ++count;
    }
}

TEST_F(LinkedListTests, Insertion) {
    lst.clear();

    lst.insert(1, lst.begin());
    lst.insert(4, ++lst.begin());
    lst.insert(2, ++lst.begin());
    lst.insert(3, ++++lst.begin());
    lst.insert(5, ++++++++lst.begin());

    int count = 1;
    for (const auto& item : lst) {
        EXPECT_EQ(count, item);
        ++count;
    }
}

TEST_F(LinkedListTests, Deletion) {
    lst.erase(++++lst.begin());

    EXPECT_EQ(1, *lst.begin());
    EXPECT_EQ(2, *++lst.begin());
    EXPECT_EQ(4, *++++lst.begin());
    EXPECT_EQ(5, *++++++lst.begin());

    lst.erase(lst.begin());

    EXPECT_EQ(2, *lst.begin());
    EXPECT_EQ(4, *++lst.begin());
    EXPECT_EQ(5, *++++lst.begin());

    auto it = lst.begin();
    while (it.next().isValid())
        ++it;
    
    lst.erase(it);

    EXPECT_EQ(2, *lst.begin());
    EXPECT_EQ(4, *++lst.begin());
}

TEST_F(LinkedListTests, RangeDeletion) {
    EXPECT_TRUE(false) << "Tests not implemented";
}