#include <gtest/gtest.h>

#include "Heap/Heap.h"

struct HeapTests : public testing::Test {
    Heap<int> heap = Heap<int>([](const int& x1, const int& x2) { return x1 < x2; });
    virtual void SetUp() override {
        heap = Heap<int>([](const int& x1, const int& x2) { return x1 < x2; });
    }

    virtual void TearDown() override {
    }
};

TEST_F(HeapTests, BasicUsage) {
    // Don't use predeclared Stuff
    Heap<double> myHeap([](const int& x1, const int& x2) { return x1 > x2; });
    myHeap.insert(5.1);
    myHeap.insert(12.3);
    myHeap.insert(3.3);

    Heap<double> copy(myHeap);

    EXPECT_EQ(myHeap.extractExtremum(), 12.3);
    EXPECT_EQ(myHeap.extractExtremum(), 5.1);
    EXPECT_EQ(myHeap.extractExtremum(), 3.3);

    copy.setComparator([](const double& x1, const double& x2) { return x1 < x2; });

    EXPECT_EQ(copy.extractExtremum(), 3.3);
    EXPECT_EQ(copy.extractExtremum(), 5.1);
    EXPECT_EQ(copy.extractExtremum(), 12.3);
}