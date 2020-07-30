# DataStructures
A C++ implementation of various data structures I learned in university. My approach (which may not be the best) was to try to give the user no access that would make them be able to break the structure. (i.e. not giving them direct access to the Nodes of a LinkedList, etc.). I have yet to exhaustively test most of these.<br/>
I am currently not happy with some of the implementations (i.e. some of the iterators).
<br/>
<br/>
I would really appreciate some feedback from anyone that may stumble on this.

## LinkedList
This template class is implemented using Nodes with std::unique_ptr<> as the next node and a raw pointer as the previous node. 
Similarly, the head of the list is a std::unique_ptr<>, while the tail is a raw pointer. 
The iterator for this class holds a raw pointer to a node and is incremented and decremented by setting the current node to the next or prev attribute of the node.
The iterator also contains a reference to the original List, which I plan to change, because that makes the iterator non-reassignable.

## BinarySearchTree
This one is implemented using std::unique_ptr<> as the left and right child and a raw pointer as the parent node.
The iterator for this class iterates through the list following either the Inorder, Preorder or Postorder Traversal. 
These are implemented as derived classes of the interface TreeTraversal, which handles most of the work of the iterators.
The iterator also contains a reference to the original List, which I plan to change, because that makes the iterator non-reassignable.

## BloomFilter
There are 2 BloomFilter implementations, which both only work for std::strings:
BloomFilter, which keeps track of the inserted elements using ints, which enables deletion from the filter, 
and SBloomFilter, which cannot be deleted from, but only uses one bit per index.
The user needs to provide the hash functions themselves. 
The reason for this is mostly that I do not think I am knowledgeable enough on the topic of hash functions to provide the best possible hashes myself.

## Heap
This is a normal heap, for which the user must specify a comparator. Note that the heaps iterator does not traverse it in order 
(only in the order the items are contained in the underlying vector)
