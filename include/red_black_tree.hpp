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
        };

    public:

    };
}

#endif //RED_BLACK_TREE_RED_BLACK_TREE_HPP
