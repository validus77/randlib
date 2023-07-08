#pragma once
#include "SkipList.h"

namespace validus_randlab {
// Skip Map class
    template <class K,
             class V,
            typename Compare         = std::less<T>,
            typename Allocator       = std::allocator<T>,
            int MAX_LEVEL = 4>
    class SkipMap {
    private:
        SkipList<std::pair<K,V>,Compare,Allocator,MAX_LEVEL> skiplist;

    public:
        using iterator = typename SkipList<std::pair<K,V>,Compare,Allocator,MAX_LEVEL>::iterator;

        explicit SkipMap()
                : skiplist(max_level, probability) {}

        void insert(const std::pair<const KeyType, ValueType> &data) {
            skiplist.insert(data);
        }

        bool search(const KeyType &key, ValueType &value) const {
            return skiplist.find(key, value) != skiplist.end();
        }

        iterator begin() {
            return skiplist.begin();
        }

        iterator end() {
            return skiplist.end();
        }
    };
}





