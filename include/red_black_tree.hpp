//
// Created by Tomáš Petříček on 03.09.2023.
//

#ifndef RED_BLACK_TREE_RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_RED_BLACK_TREE_HPP

#include <binary_search_tree.hpp>
#include <cstddef>
#include <type_traits>
#include <vector>
#include <queue>

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

            template<class U>
            friend
            class builder;

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

                // link parents
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
                // rotate
                curr->right = right_left;
                right->left = curr;

                // link root
                if (curr==this->root_) {
                    this->root_ = right;
                }
                return right;
            }

            node_ptr rotate_right(node_ptr curr)
            {
                node_ptr left{curr->left}, left_right{left->right};

                // link parents
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
                // rotate
                curr->left = left_right;
                left->right = curr;

                // link root
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

                    if (parent==grand_parent->left) {
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

            static int is_valid(const node_t* const curr)
            {
                bool valid{true};
                int right_black_count, left_black_count;
                right_black_count = left_black_count = 1;

                if (curr) {
                    if (curr->right) {
                        valid &= curr->right->value>curr->value && curr->right->parent==curr;
                        if (valid && curr->right->parent->color==colors::red) {
                            valid &= curr->right->color==colors::black;
                        }
                        if (valid) {
                            right_black_count = is_valid(curr->right);
                            valid &= right_black_count>0;
                        }
                    }
                    // symmetrical
                    if (valid && curr->left) {
                        valid &= curr->left->value<curr->value && curr->left->parent==curr;
                        if (valid && curr->left->parent->color==colors::red) {
                            valid &= curr->left->color==colors::black;
                        }
                        if (valid) {
                            left_black_count = is_valid(curr->left);
                            valid &= left_black_count>0;
                        }
                    }
                    return (valid && left_black_count==right_black_count) ? left_black_count
                            +(curr->color==colors::black) : 0;
                }
                return 1;
            }

            bool same(const node_t* const lhs, const node_t* const rhs) const
            {
                if (!lhs || !rhs) {
                    return lhs==lhs;
                }
                return (lhs->value==rhs->value && lhs->color==rhs->color) && same(lhs->right, rhs->right)
                        && same(lhs->left, rhs->left);
            }

        public:
            bool operator==(const tree& rhs) const
            {
                return same(this->root_, rhs.root_);
            }

            bool operator!=(const tree& rhs) const
            {
                return !(rhs==*this);
            }

            static bool is_valid(const tree& tree)
            {
                if (tree.root_) {
                    return tree.root_->color==colors::black && !tree.root_->parent && is_valid(tree.root_);
                }
                return true;
            }

            void insert(const T& value)
            {
                if (this->root_) {
                    node_ptr curr{this->root_}, parent{nullptr};

                    // reach leaf
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

        template<class T>
        class builder {
            const std::pair<T, colors> empty_;
            using tree_t = tree<T>;
            using node_t = typename tree_t::node_t;
            using node_ptr = typename tree_t::node_ptr;

        public:
            using data_type = std::pair<T, colors>;

            explicit builder(const T empty)
                    :empty_{empty, colors::black} { }

            const data_type& empty() const
            {
                return empty_;
            }

            tree<T> from_level_order(const std::vector<data_type>& vals) const
            {
                tree<T> tree;
                if (vals.empty()) {
                    return tree;
                }
                tree.root_ = new node_t{vals.front().first, nullptr, vals.front().second};
                std::queue<node_ptr> parents;
                parents.push(tree.root_);
                node_ptr parent;
                std::size_t i{1};

                // build
                while (!parents.empty() && i<vals.size()) {
                    parent = parents.front();
                    parents.pop();

                    // add left child
                    if (vals[i]!=empty_ && i<vals.size()) {
                        parent->left = new node_t{vals[i].first, parent, vals[i].second};
                        parents.push(parent->left);
                    }
                    i++;

                    // add right child
                    if (vals[i]!=empty_ && i<vals.size()) {
                        parent->right = new node_t{vals[i].first, parent, vals[i].second};
                        parents.push(parent->right);
                    }
                    i++;
                }
                return tree;
            }
        };
    }

    template<class T>
    using red_black_tree = red_black::tree<T>;
}

#endif //RED_BLACK_TREE_RED_BLACK_TREE_HPP
