#include <red_black_tree.hpp>
#include <string>
#include <iostream>

int main()
{
    using tree_type = top::red_black_tree<int>;
    tree_type tree;
    tree.insert(10);
    tree.insert(2);
    tree.insert(23);
    tree.insert(12);
    tree.insert(17);
    tree.insert(50);
    tree.insert(100);
    tree.insert(200);
    std::cout << std::boolalpha << tree_type::is_valid(tree) << std::endl;
    return 0;
}
