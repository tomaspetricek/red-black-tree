//
// Created by Tomáš Petříček on 03.09.2023.
//

#ifndef RED_BLACK_TREE_RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_RED_BLACK_TREE_HPP

#include <binary_search_tree.hpp>

namespace top {
    template<class T>
    class red_black_tree : public binary_search_tree<T> {
        struct node : public binary_search_tree<T>::node {
            enum colors {
                red, black
            };
            colors color;

            explicit node(T value, colors color)
                    :binary_search_tree<T>::node{value}, color{color} { }

            ~node() override = default;
        };

        void rotate_left(node* curr)
        {

        }

        void rotate_right(node* curr)
        {

        }

        void rotate_left_right(node* curr)
        {

        }

        void rotate_right_left(node* curr)
        {

        }

        node* rotate(node* curr)
        {

        }

        node* insert(node* curr, const T& value)
        {
            if (curr) {
                if (curr->value==value) {
                    return curr;
                }
                if (curr->value>value) {
                    curr->left = insert(static_cast<node*>(curr->left), value);
                }
                else {
                    curr->right = insert(static_cast<node*>(curr->right), value);
                }
                //
                return curr;
            }
            this->size_++;
            return new node{value, node::colors::red};
        }

    public:
        void insert(const T& value)
        {
            if (this->root_) {
                insert(static_cast<node*>(this->root_), value);
            }
            else {
                this->root_ = new node{value, node::colors::black};
                this->size_++;
            }
        }
    };
}

#endif //RED_BLACK_TREE_RED_BLACK_TREE_HPP
