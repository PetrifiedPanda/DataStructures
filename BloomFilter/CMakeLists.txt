cmake_minimum_required(VERSION 3.10.2)

set(This BloomFilter)

set(Sources
    BloomFilter.cpp
    SBloomFilter.cpp
)

set(Headers
    BloomFilter.h
    SBloomFilter.h
)

add_library(${This} STATIC ${Sources} ${Headers})