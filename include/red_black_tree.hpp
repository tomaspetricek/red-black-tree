//
// Created by Tomáš Petříček on 03.09.2023.
//

#ifndef RED_BLACK_TREE_RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_RED_BLACK_TREE_HPP

#include <binary_search_tree.hpp>
#include <cstddef>
#include <type_traits>

namespace top {
    namespace red_black {
        enum colors : std::underlying_type<std::byte>::type {
            red, black
        };

        template<class T>
        struct node : public binary_search::node<T, node> {
            using node_ptr = node<T>*;
            node_ptr parent{nullptr};
            colors color{colors::red};

            explicit node(const T& value, node_ptr parent, colors color = colors::red)
                    :binary_search::node<T, node>{value}, parent{parent}, color{color} { }
        };

        template<class T>
        class tree : public binary_search::tree<T, node> {
            using base_t = typename binary_search::tree<T, node>;
            using node_t = typename base_t::node_t;
            using node_ptr = typename base_t::node_ptr;

            colors get_color(const node_t* const curr)
            {
                return curr ? curr->color : colors::black;
            }

            void set_color(node_ptr curr, colors color)
            {
                if (curr) {
                    curr->color = color;
                }
            }

            node_ptr rotate_left(node_ptr curr)
            {
                node_ptr right{curr->right}, right_left{right->left};

                right->parent = curr->parent;
                if (curr->parent) {
                    if (curr->parent->right==curr) {
                        right->parent->right = right;
                    }
                    else {
                        right->parent->left = right;
                    }
                }
                curr->parent = right;
                if (right_left) {
                    right_left->parent = curr;
                }

                curr->right = right_left;
                right->left = curr;
                if (curr==this->root_) {
                    this->root_ = right;
                }
                return right;
            }

            node_ptr rotate_right(node_ptr curr)
            {
                node_ptr left{curr->left}, left_right{left->right};

                left->parent = curr->parent;
                if (curr->parent) {
                    if (curr->parent->right==curr) {
                        left->parent->right = left;
                    }
                    else {
                        left->parent->left = left;
                    }
                }
                curr->parent = left;
                if (left_right) {
                    left_right->parent = curr;
                }

                curr->left = left_right;
                left->right = curr;
                if (curr==this->root_) {
                    this->root_ = left;
                }
                return left;
            }

            void fix_after_insertion(node_ptr curr)
            {
                node_ptr parent, grand_parent, uncle;
                while (curr!=this->root_ && curr->color==colors::red && curr->parent->color==colors::red) {
                    parent = curr->parent;
                    grand_parent = parent->parent;
                    if (curr->parent==grand_parent->left) {
                        uncle = grand_parent->right;
                        if (get_color(uncle)==colors::red) {
                            parent->color = colors::black;
                            set_color(uncle, colors::black);
                            grand_parent->color = colors::red;
                            curr = grand_parent;
                        }
                        else {
                            if (curr==parent->right) {
                                parent = rotate_left(parent);
                            }
                            rotate_right(grand_parent);
                            parent->color = colors::black;
                            grand_parent->color = colors::red;
                            curr = parent;
                        }
                    }
                        // symmetrical
                    else {
                        uncle = grand_parent->left;
                        if (get_color(uncle)==colors::red) {
                            parent->color = colors::black;
                            set_color(uncle, colors::black);
                            grand_parent->color = colors::red;
                            curr = grand_parent;
                        }
                        else {
                            if (curr==parent->left) {
                                parent = rotate_right(parent);
                            }
                            rotate_left(grand_parent);
                            parent->color = colors::black;
                            grand_parent->color = colors::red;
                            curr = parent;
                        }
                    }
                }
                this->root_->color = colors::black;
            }

        public:
            void insert(const T& value)
            {
                if (this->root_) {
                    node_ptr curr{this->root_}, parent{nullptr};
                    while (curr) {
                        parent = curr;
                        if (curr->value==value) {
                            return;
                        }
                        if (curr->value>value) {
                            curr = curr->left;
                        }
                        else {
                            curr = curr->right;
                        }
                    }
                    if (parent->value>value) {
                        parent->left = new node_t{value, parent};
                        fix_after_insertion(parent->left);
                    }
                    else {
                        parent->right = new node_t{value, parent};
                        fix_after_insertion(parent->right);
                    }
                }
                else {
                    this->root_ = new node_t{value, nullptr, colors::black};
                }
                this->size_++;
            }

            void erase(const T& value)
            {

            }
        };
    }

    template<class T>
    using red_black_tree = red_black::tree<T>;
}

#endif //RED_BLACK_TREE_RED_BLACK_TREE_HPP
