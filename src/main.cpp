#include <iostream>
#include <red_black_tree.hpp>
#include <string>

int main()
{
    top::red_black_tree<int> tree;
    tree.contains(10);
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    assert(tree.contains(10) && tree.contains(20) && tree.contains(30));
    return 0;
}
