//
// Created by Tomáš Petříček on 03.09.2023.
//

#ifndef RED_BLACK_TREE_RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_RED_BLACK_TREE_HPP

#include <binary_search_tree.hpp>
#include <cstddef>
#include <type_traits>

namespace top::red_black {
    enum colors : std::underlying_type<std::byte>::type {
        red, black
    };

    template<class T>
    struct node : public binary_search::node<T, node> {
        colors color;

        explicit node(T value, colors color)
                :binary_search::node<T, node>{value}, color{color} { }
    };

    template<class T>
    class tree : public binary_search::tree<T, node> {
        using node_ptr = node<T>*;

        void rotate_left(node_ptr curr)
        {

        }

        void rotate_right(node_ptr curr)
        {

        }

        void rotate_left_right(node_ptr curr)
        {

        }

        void rotate_right_left(node_ptr curr)
        {

        }

        node_ptr rotate(node_ptr curr)
        {

        }

        node_ptr insert(node_ptr curr, const T& value)
        {
            if (curr) {
                if (curr->value==value) {
                    return curr;
                }
                if (curr->value>value) {
                    curr->left = insert(curr->left, value);
                }
                else {
                    curr->right = insert(curr->right, value);
                }
                //
                return curr;
            }
            this->size_++;
            return new node{value, colors::red};
        }

    public:
        void insert(const T& value)
        {
            if (this->root_) {
                insert(this->root_, value);
            }
            else {
                this->root_ = new node{value, colors::black};
                this->size_++;
            }
        }
    };
}

#endif //RED_BLACK_TREE_RED_BLACK_TREE_HPP
