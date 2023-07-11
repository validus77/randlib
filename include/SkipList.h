#pragma once

#include <stdlib.h>
#include <memory>
#include <iostream>
#include <random>
#include <functional>
#include <iterator>
#include <utility>

namespace validus_randlab {
// Node class for Skip List
    template <class T>
    class SkipNode {
    public:
        T value;
        SkipNode** forward;

        explicit SkipNode(int level, T value) {
            this->value = value;
            forward = new SkipNode*[level + 1];
            memset(forward, 0, sizeof(SkipNode*) * (level + 1));
        }

        ~SkipNode() {
            delete[] forward;
        }
    };

    template <class T>
    class SkipListIterator : public std::iterator<std::forward_iterator_tag, T> {
    private:
        SkipNode<T>* current;

    public:
        explicit SkipListIterator(SkipNode<T>* node = nullptr) : current(node) {}

        int& operator*() const {
            return current->value;
        }

        bool operator==(const SkipListIterator &other) const {
            return current == other.current;
        }

        bool operator!=(const SkipListIterator &other) const {
            return !(*this == other);
        }

        SkipListIterator operator+(int n) const {
            SkipListIterator iter = *this;
            for (int i = 0; i < n; ++i) {
                if (!iter.current) {
                    throw std::out_of_range("Iterator advanced past end.");
                }
                iter.current = iter.current->forward[0];
            }
            return iter;
        }

        SkipListIterator &operator+=(int n) {
            for (int i = 0; i < n; ++i) {
                if (!this->current) {
                    throw std::out_of_range("Iterator advanced past end.");
                }
                this->current = this->current->forward[0];
            }
            return *this;
        }

        SkipListIterator &operator++() {
            if (current)
                current = current->forward[0];
            return *this;
        }

        SkipListIterator operator++(int) {
            SkipListIterator temp(*this);
            ++(*this);
            return temp;
        }
    };

// Skip List class
    template <class T,
            typename Compare         = std::less<T>,
            typename Allocator       = std::allocator<T>,
            int MAX_LEVEL = 4>
    class SkipList {
    public:
        typedef T                                           value_type;

        typedef Allocator                                   allocator_type;
        typedef typename allocator_type::difference_type    difference_type;
        typedef typename allocator_type::reference          reference;
        typedef typename allocator_type::const_reference    const_reference;
        typedef typename allocator_type::pointer            pointer;
        typedef typename allocator_type::const_pointer      const_pointer;
        typedef Compare                                     compare;


        typedef SkipListIterator<T>                         iterator;
        typedef const SkipListIterator<T>                         const_iterator;

    private:
        using NodeAllocator = typename std::allocator_traits<allocator_type>::template rebind_alloc<SkipNode<T>>;

        float P;
        int level;
        size_t size = 0;
        SkipNode<T> *header;
        NodeAllocator nodeAllocator;

    public:
        explicit SkipList()
                : SkipList(0.5) {
        }

        // Defaulted copy constructor
        SkipList(const SkipList &) = default;

        // Defaulted move constructor
        SkipList(SkipList &&) noexcept = default;

        explicit SkipList(float probability)
                : P(probability), level(0) {
            header = nodeAllocator.allocate(1);
            nodeAllocator.construct(header, MAX_LEVEL, T());
        }

        ~SkipList() {
            SkipNode<T> *current = header->forward[0];
            while (current) {
                SkipNode<T> *temp = current;
                current = current->forward[0];
                nodeAllocator.destroy(temp);
                nodeAllocator.deallocate(temp, 1);
            }
            nodeAllocator.destroy(header);
            nodeAllocator.deallocate(header, 1);
        }

        // Insert a value into the Skip List
        iterator insert(T value) {
            SkipNode<T>* current = header;

            SkipNode<T>* update[MAX_LEVEL + 1];
            memset(update, 0, sizeof(SkipNode<T>*) * (MAX_LEVEL + 1));

            // Traverse the Skip List and update the update[] array
            for (int i = level; i >= 0; i--) {
                while (current->forward[i] &&  compare{}(current->forward[i]->value, value)) {
                    current = current->forward[i];
                }
                update[i] = current;
            }

            current = current->forward[0];

            // If the value already exists, return
            if (current && current->value == value) {
                return iterator(current);
            }

            // Generate a random level for the new node
            int new_level = randomLevel();

            // If the new level is greater than the current level, update the update[] array
            if (new_level > level) {
                for (int i = level + 1; i <= new_level; i++) {
                    update[i] = header;
                }
                level = new_level;
            }

            // Create a new node with the random level
            auto* new_node = nodeAllocator.allocate(1);
            nodeAllocator.construct(new_node, new_level, value);

            // Insert the new node into the Skip List and update the forward pointers
            for (int i = 0; i <= new_level; i++) {
                new_node->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = new_node;
            }
            size++;
            return iterator(new_node);
        }

        // Search for a value in the Skip List
        iterator find(T& value) {
            SkipNode<T>* current = header;

            // Traverse the Skip List and update the current pointer
            for (int i = level; i >= 0; i--) {
                while (current->forward[i] && current->forward[i]->value < value) {
                    current = current->forward[i];
                }
            }

            current = current->forward[0];

            // If the current node contains the searched value, print a success message
            if (current && current->value == value) {
                return iterator(current);
            }
            return end();
        }

        const_iterator find(const T& value) const {
            SkipNode<T>* current = header;

            // Traverse the Skip List and update the current pointer
            for (int i = level; i >= 0; i--) {
                while (current->forward[i] && current->forward[i]->value < value) {
                    current = current->forward[i];
                }
            }

            current = current->forward[0];

            // If the current node contains the searched value, print a success message
            if (current && current->value == value) {
                return const_iterator(current);
            }
            return end();
        }

        iterator erase(iterator pos) {
            return erase_impl(pos);
        }

        iterator erase(iterator first, iterator last) {
            return erase_range_impl(first, last);
        }

        size_t getSize() const {
            return size;
        }

        void changeNewLevelProbability(float probability) {
            P = probability;
        }

        T &operator[](int index) {
            SkipNode<T> *current = header->forward[0];
            int i = 0;

            while (current) {
                if (i == index)
                    return current->value;

                current = current->forward[0];
                i++;
            }

            throw std::out_of_range("Index out of range");
        }

        const T& operator[](int index) const {
            SkipNode<T>* current = header->forward[0];
            int i = 0;

            while (current) {
                if (i == index)
                    return current->value;

                current = current->forward[0];
                i++;
            }

            throw std::out_of_range("Index out of range");
        }

        // Begin iterator
        iterator begin() {
            return iterator(header->forward[0]);
        }

        // End iterator
        iterator end() {
            return iterator(nullptr);
        }

        // Const begin iterator
        const_iterator begin() const {
            return const_iterator(header->forward[0]);
        }

        // Const end iterator
        const_iterator end() const {
            return const_iterator(nullptr);
        }

    private:
        iterator erase_impl(iterator pos) {
            if (pos == end()) {
                throw std::out_of_range("Cannot erase end iterator");
            }

            SkipNode<T> *node = header;
            SkipNode<T> *to_delete = nullptr;
            SkipNode<T> *update[MAX_LEVEL + 1];
            memset(update, 0, sizeof(SkipNode<T> *) * (MAX_LEVEL + 1));

            for (int i = level; i >= 0; --i) {
                while (node->forward[i] != nullptr && compare{}(node->forward[i]->value, *pos)) {
                    node = node->forward[i];
                }
                update[i] = node;
            }

            node = node->forward[0];
            if (node != nullptr && node->value == *pos) {
                for (int i = 0; i <= level; ++i) {
                    if (update[i]->forward[i] != node) {
                        break;
                    }
                    update[i]->forward[i] = node->forward[i];
                }

                to_delete = node;
                node = node->forward[0];
                while (level > 0 && header->forward[level] == nullptr) {
                    level--;
                }
                size--;
            }

            nodeAllocator.destroy(to_delete);
            nodeAllocator.deallocate(to_delete, 1);
            return iterator(node);
        }

        iterator erase_range_impl(iterator first, iterator last) {
            while (first != last) {
                first = erase_impl(first);
            }
            return first;
        }

        // Generate a random level for a new node
        [[nodiscard]] int randomLevel() const {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(0, 1);
            int lvl = 0;
            while (dis(gen) < P && lvl < MAX_LEVEL) {
                lvl++;
            }
            return lvl;
        }


    };
}
