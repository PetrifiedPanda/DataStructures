# DataStructures
A C++ implementation of various data structures I learned in university. My approach (which may not be the best) was to try to give the user no access that would make them be able to break the structure. (i.e. not giving them direct access to the Nodes of a LinkedList, etc.). 
<br/>
I have yet to exhaustively test most of these and am currently not happy with some of the implementations (i.e. some of the iterators).
<br/>
<br/>
I would really appreciate some feedback from anyone that may stumble on this.

## LinkedList
This template class is implemented using Nodes with std::unique_ptr<> as the next node and a raw pointer as the previous node. 
Similarly, the head of the list is a std::unique_ptr<>, while the tail is a raw pointer. 
The iterator for this class holds a raw pointer to a node and is incremented and decremented by setting the current node to the next or prev attribute of the node.
The iterator also contains a reference to the original List, which I plan to change, because that makes the iterator non-reassignable.
<br/>
SizeLinkedList is a small attempt at making a LinkedList that saves its size in a variable. I did not work on that one too much.

## BinarySearchTree
The Core of all Data Structures in this folder is BSTBase, which is an almost complete Binary-Search-Tree implementation. It receives the Node type as a template parameter, so you can derive Trees with different Nodes from it. The Node type must use unique_ptr for the left and right children, and a raw pointer for the parent.
<br/>
BinarySearchTree is just a redefinition of BSTBase with an ordinary implementation of the Nodes.
<br/>
SplayTree is a Splay Tree implementation that inherits from BinarySearchTree
<br/>
RedBlackTree is a Red-Black-Tree implementation. The deletion does not work correctly yet. (And the actual reason BSTBase is structured in the way that it is)
<br/>
All of these Trees use BSTBaseIt as their iterator, with which you can traverse the tree as you would with a node pointer.

## BloomFilter
There are 2 BloomFilter implementations, which both only work for std::strings:
BloomFilter, which keeps track of the inserted elements using ints, which enables deletion from the filter, 
and SBloomFilter, which cannot be deleted from, but only uses one bit per index.
The user needs to provide the hash functions themselves. 
The reason for this is mostly that I do not think I am knowledgeable enough on the topic of hash functions to provide the best possible hashes myself.

## Heap
This is a normal heap, for which the user must specify a comparator. Note that the heaps iterator does not traverse it in order 
(only in the order the items are contained in the underlying vector)
