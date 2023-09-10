//
// Created by Tomáš Petříček on 03.09.2023.
//

#ifndef RED_BLACK_TREE_RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_RED_BLACK_TREE_HPP

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
        struct node {
            using node_ptr = node<T>*;

            T value;
            node_ptr right{nullptr}, left{nullptr}, parent{nullptr};
            colors color{colors::red};

            explicit node(const T& value)
                    :value(value) { }

            explicit node(const T& value, node_ptr parent, colors color = colors::red)
                    :value{value}, parent{parent}, color{color} { }
        };

        template<class T>
        class tree {
            using node_t = node<T>;
            using node_ptr = node_t*;

            template<class U>
            friend
            class builder;

            node_ptr root_{nullptr};
            std::size_t size_{0};

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
                if (curr==root_) {
                    root_ = right;
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
                if (curr==root_) {
                    root_ = left;
                }
                return left;
            }

            void fix_after_insertion(node_ptr curr)
            {
                node_ptr parent, grand_parent, uncle;
                while (curr!=root_ && curr->color==colors::red && curr->parent->color==colors::red) {
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
                root_->color = colors::black;
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

            bool same(const tree& rhs, const tree& lhs) const
            {
                return rhs.size_==lhs.size_ && same(rhs.root_, lhs.root_);
            }

            void clear(node_ptr& curr)
            {
                if (curr) {
                    clear(curr->left);
                    clear(curr->right);
                    delete curr;
                }
            }

            static node_ptr copy(const node_t* const src)
            {
                if (src) {
                    auto dest = new node_t{*src};
                    if (src->left) {
                        dest->left = copy(src->left);
                        dest->left->parent = dest;
                    }
                    if (src->right) {
                        dest->right = copy(src->right);
                        dest->right->parent = dest;
                    }
                    return dest;
                }
                return nullptr;
            }

        public:
            class builder {
                const std::pair<T, colors> empty_;

            public:
                using data_type = std::pair<T, colors>;

                explicit builder(const T empty)
                        :empty_{empty, colors::black} { }

                const data_type& empty() const
                {
                    return empty_;
                }

                tree from_level_order(const std::vector<data_type>& vals) const
                {
                    tree tree;
                    if (vals.empty()) {
                        return tree;
                    }
                    tree.root_ = new node_t{vals.front().first, nullptr, vals.front().second};
                    tree.size_++;
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
                            tree.size_++;
                        }
                        i++;

                        // add right child
                        if (vals[i]!=empty_ && i<vals.size()) {
                            parent->right = new node_t{vals[i].first, parent, vals[i].second};
                            parents.push(parent->right);
                            tree.size_++;
                        }
                        i++;
                    }
                    return tree;
                }
            };

            using value_type = T;
            using size_type = std::size_t;

            tree() noexcept = default;

            tree(const tree& src)
                    :root_{copy(src.root_)}, size_{src.size_} { }

            tree& operator=(const tree& rhs)
            {
                if (&rhs!=this) {
                    // copy-and-swap idiom
                    tree temp{rhs};
                    swap(temp);
                }
                return *this;
            }

            tree(tree&& src) noexcept
                    :root_{src.root_}, size_{src.size_}
            {
                src.root_ = nullptr;
                src.size_ = 0;
            }

            tree& operator=(tree&& rhs)
            {
                if (&rhs!=this) {
                    tree temp{std::move(rhs)};
                    swap(temp);
                }
                return *this;
            }

            void swap(tree& other) noexcept
            {
                std::swap(root_, other.root_);
                std::swap(size_, other.size_);
            }

            bool operator==(const tree& rhs) const
            {
                return same(*this, rhs);
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
                if (root_) {
                    node_ptr curr{root_}, parent{nullptr};
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

                    // add node
                    auto child = new node_t{value, parent};
                    if (parent->value>value) {
                        parent->left = child;
                    }
                    else {
                        parent->right = child;
                    }
                    fix_after_insertion(child);
                }
                else {
                    root_ = new node_t{value, nullptr, colors::black};
                }
                size_++;
            }

            void erase(const T& value)
            {

            }

            bool contains(const T& value) const
            {
                node_ptr curr{root_};
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

            ~tree()
            {
                clear();
            }
        };
    }

    template<class T>
    using red_black_tree = red_black::tree<T>;
}

#endif //RED_BLACK_TREE_RED_BLACK_TREE_HPP
