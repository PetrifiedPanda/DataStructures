#pragma once

#include <functional>
#include <vector>

class BloomFilter {
    std::vector<unsigned int> data_;
    std::vector<std::function<size_t(const char&)>> hashes_;

   public:
    BloomFilter(std::vector<std::function<size_t(const char&)>> hashes, size_t initSize) : data_(initSize, false), hashes_(std::move(hashes)) {}
    BloomFilter(const BloomFilter& filter) : data_(filter.data_), hashes_(filter.hashes_){};
    BloomFilter(BloomFilter&& filter) noexcept : data_(std::move(filter.data_)), hashes_(std::move(filter.hashes_)) {}

    BloomFilter& operator=(const BloomFilter& filter);
    BloomFilter& operator=(BloomFilter&& filter);

    void add(const std::string& value);
    void remove(const std::string& value);

    bool contains(const std::string& value);
};