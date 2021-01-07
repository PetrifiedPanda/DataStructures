cmake_minimum_required(VERSION 3.10.2)

set(This DataStructures)

project(${This})

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(googletest)

enable_testing()

add_subdirectory(LinkedList)
add_subdirectory(Heap)
add_subdirectory(BloomFilter)
add_subdirectory(BinarySearchTree)
add_subdirectory(Trie)

add_library(${This} INTERFACE)
target_link_libraries(${This} INTERFACE BinarySearchTree BloomFilter Heap LinkedList Trie)
target_include_directories(${This} INTERFACE ./)

add_subdirectory(test)