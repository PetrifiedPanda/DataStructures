#include <gtest/gtest.h>

#include "Heap/Heap.h"

struct HeapTests : public testing::Test {
    Heap<int> heap = Heap<int>();
    virtual void SetUp() override {
        
    }

    virtual void TearDown() override {
    }
};

TEST_F(HeapTests, BasicUsage) {
    // Don't use predeclared Stuff
    Heap<double, std::greater<double>> myHeap = Heap<double, std::greater<double>>();
    myHeap.insert(5.1);
    myHeap.insert(12.3);
    myHeap.insert(3.3);

    Heap<double, std::greater<double>> copy(myHeap);

    EXPECT_EQ(myHeap.extractExtremum(), 12.3);
    EXPECT_EQ(myHeap.extractExtremum(), 5.1);
    EXPECT_EQ(myHeap.extractExtremum(), 3.3);
}