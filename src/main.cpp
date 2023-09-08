#include <red_black_tree.hpp>
#include <string>

int main()
{
    top::red_black_tree<int> tree;
    tree.insert(10);
    tree.insert(2);
    tree.insert(23);
    tree.insert(12);
    tree.insert(17);
    tree.insert(50);
    tree.insert(100);
    tree.insert(200);
    return 0;
}
