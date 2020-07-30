#include "SBloomFilter.h"

SBloomFilter& SBloomFilter::operator=(const SBloomFilter& filter) {
    data_ = filter.data_;
    hashes_ = filter.hashes_;
}

SBloomFilter& SBloomFilter::operator=(SBloomFilter&& filter) {
    data_ = std::move(filter.data_);
    hashes_ = std::move(filter.hashes_);
}

void SBloomFilter::add(const std::string& word) {
    for (const char& c : word) {
        for (const auto& func : hashes_) {
            size_t bitLocation = func(c);
            size_t location = bitLocation / 8;
            unsigned char* arr = &data_[location];
            *arr |= 1 << (bitLocation - location * 8);  // Set the bit to 1
        }
    }
}

bool SBloomFilter::contains(const std::string& word) {
    bool found = true;
    for (const char& c : word) {
        for (const auto& func : hashes_) {
            size_t bitLocation = func(c);
            size_t location = bitLocation / 8;
            unsigned char* arr = &data_[location];
            found &= static_cast<bool>(*arr & (1 << (bitLocation - location * 8)));
            if (!found)
                return found;
        }
    }

    return found;
}