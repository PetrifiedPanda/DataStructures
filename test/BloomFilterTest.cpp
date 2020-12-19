#include <gtest/gtest.h>

#include "BloomFilter/BloomFilter.h"
#include "BloomFilter/SBloomFilter.h"

struct BloomFilterTests : public testing::Test {
    std::vector<std::string> strings = {
        "Data Structures",
        "A random String",
        "Bloom filter",
        "aidj3928fj2jisojdaijd092"
    };

    virtual void SetUp() override {
    }

    virtual void TearDown() override {
    }
};

size_t hash1(const char& c) {
    return c % 7;
}

size_t hash2(const char& c) {
    return c % 47;
}

size_t hash3(const char& c) {
    return c % 97;
}

TEST_F(BloomFilterTests, BloomFilter) {
    BloomFilter filter({hash1, hash2, hash3}, 1000);

    for (const std::string& str : strings)
        filter.add(str);

    for (const std::string& str : strings)
        EXPECT_TRUE(filter.contains(str));

    BloomFilter copy(filter);

    for (const std::string& str : strings)
        EXPECT_TRUE(copy.contains(str));

    for (const std::string& str : strings) {
        filter.remove(str);
        EXPECT_FALSE(filter.contains(str));
    }
}

TEST_F(BloomFilterTests, SBloomFilter) {
    SBloomFilter filter({hash1, hash2, hash3}, 1000);
    for (const std::string& str : strings)
        filter.add(str);

    for (const std::string& str : strings)
        EXPECT_TRUE(filter.contains(str));

    SBloomFilter copy(filter);

    for (const std::string& str : strings)
        EXPECT_TRUE(copy.contains(str));
}