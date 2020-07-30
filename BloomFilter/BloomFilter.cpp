#include "BloomFilter.h"

BloomFilter& BloomFilter::operator=(const BloomFilter& filter) {
    data_ = filter.data_;
    hashes_ = filter.hashes_;
}

BloomFilter& BloomFilter::operator=(BloomFilter&& filter) {
    data_ = std::move(filter.data_);
    hashes_ = std::move(filter.hashes_);
}

void BloomFilter::add(const std::string& word) {
    for (const char& c : word) {
        for (const auto& func : hashes_)
            ++data_[func(c)];
    }
}

void BloomFilter::remove(const std::string& word) {
    for (const char& c : word) {
        for (const auto& func : hashes_)
            --data_[func(c)];
    }
}

bool BloomFilter::contains(const std::string& word) {
    bool found = true;
    for (const char& c : word) {
        for (const auto& func : hashes_) {
            found &= static_cast<bool>(data_[func(c)]);
            if (!found)
                return found;
        }
    }

    return found;
}