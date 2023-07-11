#pragma once
#include "SkipList.h"

namespace validus_randlab {
    // Skip Map Pair
    template<class K, class V>
    class SkipMapPair {
    private:
        K key;
        V value;
    public:
        // Default constructor
        SkipMapPair() {}

        // Constructor with key and value
        SkipMapPair(const K &key, const V &value)
                : key(key), value(value) {}

        // Defaulted copy constructor
        SkipMapPair(const SkipMapPair &) = default;

        // Defaulted move constructor
        SkipMapPair(SkipMapPair &&) noexcept = default;

        // Assignment operator
        SkipMapPair &operator=(const SkipMapPair &other) {
            if (this != &other) {
                key = other.key;
                value = other.value;
            }
            return *this;
        }

        // Less than comparison operator
        bool operator<(const SkipMapPair &other) const {
            return key < other.key;
        }

        // Equality comparison operator
        bool operator==(const SkipMapPair &other) const {
            return key == other.key && value == other.value;
        }

        // Output stream operator
        friend std::ostream &operator<<(std::ostream &os, const SkipMapPair &pair) {
            os << "(" << pair.key << ", " << pair.value << ")";
            return os;
        }
    };

// Skip Map class
    template<class K,
            class V,
            typename Compare         = std::less<T>,
            typename Allocator       = std::allocator<T>,
            int MAX_LEVEL = 4>
    class SkipMap {
    private:
        using SkipMapPair = SkipMapPair<K, V>;
        SkipList<SkipMapPair, Compare, Allocator, MAX_LEVEL> skiplist;

    public:
        using iterator = typename SkipList<std::pair<K, V>, Compare, Allocator, MAX_LEVEL>::iterator;

        explicit SkipMap() {}

        void insert(const SkipMapPair &data) {
            skiplist.insert(data);
        }

        bool find(const K &key) const {
            return skiplist.find(key) != skiplist.end();
        }

        iterator begin() {
            return skiplist.begin();
        }

        iterator end() {
            return skiplist.end();
        }
    };
}





