//
// Created by Tomáš Petříček on 03.09.2023.
//

#ifndef RED_BLACK_TREE_BINARY_SEARCH_TREE_HPP
#define RED_BLACK_TREE_BINARY_SEARCH_TREE_HPP

#include <cstddef>

namespace top {
    template<class T>
    class binary_search_tree {
    protected:
        struct node {
            T value;
            node* right{nullptr}, * left{nullptr};

            explicit node(T value)
                    :value(value) { }

            virtual ~node() = default;
        };

        node* root_{nullptr};
        std::size_t size_{0};

        bool same(node* lhs, node* rhs)
        {
            if (!lhs || !rhs) {
                return lhs==lhs;
            }
            return (lhs->value==rhs->value) && same(lhs->right, rhs->right) && same(lhs->left, rhs->left);
        }

        void clear(node*& curr)
        {
            if (curr) {
                clear(curr->left);
                clear(curr->right);
                delete curr;
            }
        }

    public:
        using value_type = T;
        using size_type = std::size_t;

        bool operator==(const binary_search_tree& rhs) const
        {
            return same(root_, rhs.root_);
        }

        bool operator!=(const binary_search_tree& rhs) const
        {
            return !(rhs==*this);
        }

        bool contains(const T& value) const
        {
            node* curr{root_};
            while (curr) {
                if (curr->value==value) {
                    return true;
                }
                if (curr->value>value) {
                    curr = curr->left;
                }
                else {
                    curr = curr->right;
                }
            }
            return false;
        }

        void clear()
        {
            clear(root_);
            root_ = nullptr;
            size_ = 0;
        }

        std::size_t size() const
        {
            return size_;
        }

        bool empty() const
        {
            return !root_;
        }

        ~binary_search_tree()
        {
            clear();
        }
    };
}

#endif //RED_BLACK_TREE_BINARY_SEARCH_TREE_HPP
