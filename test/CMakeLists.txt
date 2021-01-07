cmake_minimum_required(VERSION 3.10.2)

set(This DataStructuresTest)

set(Sources
    TestMain.cpp
    LinkedListTest.cpp
    SizeLinkedListTest.cpp
    BloomFilterTest.cpp
    BinarySearchTreeTest.cpp
    HeapTest.cpp
    SplayTreeTest.cpp
    RedBlackTreeTest.cpp
    TrieTest.cpp
)

add_executable(${This} ${Sources})
target_link_libraries(${This}
    gtest_main
    DataStructures
)

add_test(
    NAME ${This}
    COMMAND ${This}
)