#include <gtest/gtest.h>

#include "Trie/Trie.h"

struct TrieTests : public testing::Test {
    Trie<char> trie;

    
    std::vector<std::string> toInsert = {"Hello", 
                                         "Hello, World",
                                         "Hellworld",
                                         "Test", 
                                         "This is a long string to see if long strings also work",
                                         "Banana",
                                         "Word"};

    virtual void SetUp() override {
        for (const auto& item : toInsert)
            trie.insert(item);
    }

    virtual void TearDown() override {
    }
};

TEST_F(TrieTests, BasicUsage) {
    std::string toDelete = "Hello, Wor";
    
    trie.insert(toDelete);

    for (const auto& item : toInsert)
        EXPECT_TRUE(trie.containsKey(item));

    EXPECT_TRUE(trie.containsKey(toDelete));

    trie.erase(toDelete);

    EXPECT_FALSE(trie.containsKey(toDelete));

    for (const auto& item : toInsert)
        EXPECT_TRUE(trie.containsKey(item));

    std::vector<std::string> coll = trie.keysWithPrefix<std::string, std::vector<std::string>>("Hell");

    EXPECT_EQ(coll.size(), 3);
    EXPECT_EQ(coll[0], toInsert[2]);
    EXPECT_EQ(coll[1], toInsert[0]);
    EXPECT_EQ(coll[2], toInsert[1]);
}

TEST_F(TrieTests, Constructors) {
    Trie<char> copy(trie);

    for (const auto& item : toInsert) {
        ASSERT_TRUE(trie.contains(item));
        ASSERT_TRUE(copy.contains(item));
    }

    Trie<char> move(std::move(copy));

    for (const auto& item : toInsert) {
        ASSERT_TRUE(trie.contains(item));
        ASSERT_TRUE(move.contains(item));
    }
}

TEST_F(TrieTests, Search) {
    std::string nonKeySeq = "Hell";
    EXPECT_TRUE(trie.contains(nonKeySeq));

    EXPECT_FALSE(trie.containsKey(nonKeySeq));

    auto it = trie.find(nonKeySeq);
    auto invalidIt = trie.findKey(nonKeySeq);
    
    EXPECT_FALSE(invalidIt.isValid());
    EXPECT_EQ(it.sequence(), nonKeySeq);

    it.enter('o');
    EXPECT_TRUE(it.isValid());
    EXPECT_TRUE(it.isKey());

    auto keyIt = trie.findKey<std::string>("Hello"); // Be careful with string literals
    EXPECT_TRUE(keyIt.isValid());
    EXPECT_TRUE(keyIt.isKey());

    EXPECT_EQ(keyIt, it);
    keyIt.removeLast();
    it.removeLast();

    EXPECT_EQ(keyIt, it);

    std::string movedSeq = keyIt.moveSequence();
    EXPECT_FALSE(keyIt.isValid());
    EXPECT_FALSE(keyIt.isKey()); // Check if isKey() does not crash when used on an empty iterator
    
    EXPECT_EQ(movedSeq, it.sequence());
}