//
// Created by Tomáš Petříček on 03.09.2023.
//

#ifndef TEST_RED_BLACK_TREE_RED_BLACK_TREE_HPP
#define TEST_RED_BLACK_TREE_RED_BLACK_TREE_HPP

#include <boost/test/unit_test.hpp>
#include <red_black_tree.hpp>
#include <limits>
#include <random>
#include <vector>
#include <numeric>
#include <algorithm>

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

    BOOST_AUTO_TEST_CASE(move_constructor_test)
    {
        tree_type src;
        src.insert(10);
        src.insert(20);
        tree_type dest{std::move(src)};
        BOOST_REQUIRE(src.empty());
        BOOST_REQUIRE_EQUAL(src.size(), 0);
        BOOST_REQUIRE(src==builder.from_level_order({}));
        BOOST_REQUIRE(dest.contains(10) && dest.contains(20));
    }

    BOOST_AUTO_TEST_CASE(move_assignment_test)
    {
        tree_type src;
        src.insert(10);
        src.insert(20);
        tree_type dest;
        dest = std::move(src);
        BOOST_REQUIRE(src.empty());
        BOOST_REQUIRE_EQUAL(src.size(), 0);
        BOOST_REQUIRE(src==builder.from_level_order({}));
        BOOST_REQUIRE(dest.contains(10) && dest.contains(20));
    }

    BOOST_AUTO_TEST_CASE(copy_constructor_test)
    {
        tree_type src;
        src.insert(10);
        src.insert(20);
        src.insert(30);
        tree_type dest{src};
        BOOST_REQUIRE(src==dest);
    }

    BOOST_AUTO_TEST_CASE(copy_assignemnt_test)
    {
        tree_type src;
        src.insert(10);
        src.insert(20);
        src.insert(30);
        tree_type dest;
        dest = src;
        BOOST_REQUIRE(src==dest);
    }

    BOOST_AUTO_TEST_CASE(empty_test)
    {
        tree_type tree;
        BOOST_REQUIRE(tree.empty());
        tree.insert(10);
        BOOST_REQUIRE(!tree.empty());
        tree.clear();
        BOOST_REQUIRE(tree.empty());
    }

    BOOST_AUTO_TEST_CASE(size_test)
    {
        tree_type tree;
        std::size_t size{0};
        BOOST_REQUIRE_EQUAL(tree.size(), size);
        tree.insert(10), size++;
        BOOST_REQUIRE_EQUAL(tree.size(), size);
        tree.insert(20), size++;
        BOOST_REQUIRE_EQUAL(tree.size(), size);
        tree.insert(30), size++;
        BOOST_REQUIRE_EQUAL(tree.size(), size);
    }

    BOOST_AUTO_TEST_CASE(clear_test)
    {
        tree_type tree;
        value_type val{10};
        tree.insert(val);
        BOOST_REQUIRE(tree.contains(val));
        BOOST_REQUIRE_EQUAL(tree.size(), 1);
        tree.clear();
        BOOST_REQUIRE(!tree.contains(val));
        BOOST_REQUIRE_EQUAL(tree.size(), 0);
        BOOST_REQUIRE(tree==tree_type{});
    }

    BOOST_AUTO_TEST_CASE(contains_test)
    {
        tree_type tree;
        std::vector<value_type> vals(100);
        std::iota(vals.begin(), vals.end(), -50);
        std::mt19937 gen(std::random_device{}());
        std::size_t size{0};

        for (std::size_t i{0}; i<100; i++) {
            size = 0;
            tree.clear();
            std::shuffle(vals.begin(), vals.end(), gen);

            for (auto val: vals) {
                tree.insert(val);
                BOOST_REQUIRE(tree.contains(val));
                BOOST_REQUIRE_EQUAL(tree.size(), (++size));
            }
        }
    }

    BOOST_AUTO_TEST_CASE(insert_duplicate_test)
    {
        tree_type tree;
        value_type val{10};
        std::size_t size{0};
        BOOST_REQUIRE(!tree.contains(val));
        BOOST_REQUIRE_EQUAL(tree.size(), size);
        tree.insert(val), size++;
        BOOST_REQUIRE(tree.contains(val));
        BOOST_REQUIRE_EQUAL(tree.size(), size);
        tree.insert(val);
        BOOST_REQUIRE(tree.contains(val));
        BOOST_REQUIRE_EQUAL(tree.size(), size);
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
