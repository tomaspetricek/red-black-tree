//
// Created by Tomáš Petříček on 03.09.2023.
//

#ifndef TEST_RED_BLACK_TREE_RED_BLACK_TREE_HPP
#define TEST_RED_BLACK_TREE_RED_BLACK_TREE_HPP

#include <boost/test/unit_test.hpp>
#include <red_black_tree.hpp>
#include <limits>
#include <random>

BOOST_AUTO_TEST_SUITE(red_black_tree_test)
    using value_type = int;
    using tree_type = top::red_black_tree<value_type>;
    using builder_type = tree_type::builder;
    builder_type builder{std::numeric_limits<int>::quiet_NaN()};
    auto empty{builder.empty()};

    std::vector<value_type> generate_samples(std::size_t size, value_type min, value_type max)
    {
        std::uniform_int_distribution<value_type> dist{min, max};
        std::default_random_engine gen;
        std::vector<value_type> samples(size);
        std::generate(samples.begin(), samples.end(), [&]() { return dist(gen); });
        return samples;
    }

    BOOST_AUTO_TEST_CASE(default_constructor_test)
    {
        tree_type actual_tree;
        BOOST_REQUIRE(builder.from_level_order({})==actual_tree);
        BOOST_REQUIRE(actual_tree.empty());
        BOOST_REQUIRE(!actual_tree.size());
    }

    BOOST_AUTO_TEST_CASE(clear_test) {

    }

    BOOST_AUTO_TEST_CASE(contains_test) {

    }

    BOOST_AUTO_TEST_CASE(insert_duplicate_test)
    {

    }

    BOOST_AUTO_TEST_CASE(random_insertion_test)
    {
        auto samples{generate_samples(1'000, -100, 100)};
        std::mt19937 gen(std::random_device{}());
        tree_type tree;

        for (std::size_t i{0}; i<1'000; i++) {
            for (auto sample: samples) {
                tree.insert(sample);
                BOOST_REQUIRE(tree_type::is_valid(tree));
            }
            tree.clear();
            std::shuffle(samples.begin(), samples.end(), gen);
        }
    }
BOOST_AUTO_TEST_SUITE_END()
#endif //TEST_RED_BLACK_TREE_RED_BLACK_TREE_HPP
