#pragma once

#include "LinkedList.h"

template <typename T>
class ListNode {
   public:
    T key;
    std::unique_ptr<ListNode<T>> next;
    ListNode<T>* prev;

    ListNode(T _key) : key(_key), next(nullptr), prev(nullptr) {}
};