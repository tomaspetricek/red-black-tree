#include <red_black_tree.hpp>
#include <limits>


int main()
{
    using value_type = int;
    using tree_type = top::red_black_tree<value_type>;
    std::vector<value_type> nums{10, 2, 23, 12, 17, 50, 100, 200};
    tree_type actual_tree;

    for (auto num: nums) {
        actual_tree.insert(num);
        assert(tree_type::is_valid(actual_tree));
    }
    using namespace top::red_black;
    using builder_t = tree<value_type>::builder;
    builder_t builder{std::numeric_limits<int>::quiet_NaN()};
    auto empty{builder.empty()};
    std::vector<builder_t::data_type> data{
            {
                    {17, colors::black},
                    {10, colors::red},
                    {50, colors::red},
                    {2, colors::black},
                    {12, colors::black},
                    {23, colors::black},
                    {100, colors::black},
                    empty, empty, empty,
                    empty, empty, empty, empty,
                    {200, colors::red},
            }
    };
    auto expect_tree = builder.from_level_order(data);
    assert(expect_tree==actual_tree);
    auto copy{expect_tree};
    assert(expect_tree==copy);
    return 0;
}
