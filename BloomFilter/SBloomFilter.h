#pragma once

#include <bitset>
#include <cmath>
#include <functional>
#include <vector>

class SBloomFilter {
    std::vector<std::function<size_t(const char&)>> hashes_;
    std::vector<unsigned char> data_;

   public:
    SBloomFilter(std::vector<std::function<size_t(const char&)>> hashes, size_t initSize) : data_(ceil(initSize / 8.0), 0), hashes_(std::move(hashes)) {}
    SBloomFilter(const SBloomFilter& filter) : data_(filter.data_), hashes_(filter.hashes_) {}
    SBloomFilter(SBloomFilter&& filter) : data_(std::move(filter.data_)), hashes_(std::move(filter.hashes_)) {}

    SBloomFilter& operator=(const SBloomFilter& filter);
    SBloomFilter& operator=(SBloomFilter&& filter);

    void add(const std::string& word);

    bool contains(const std::string& word);
};