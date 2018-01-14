/*  node_tree.cpp - node tree tests
 *  Copyright (C) 2017-2018 caryoscelus
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <catch.hpp>

#include <core/node_tree/traverse.h>
#include "new_node.h"

using namespace rainynite;
using namespace rainynite::core;

TEST_CASE("Node tree index", "[node]") {
    auto root = make_shared<Add>();
    auto mid = make_shared<Add>();
    auto leaf = mid->get_link(0);
    root->set_link(0, mid);

    auto tree = NodeTree(root);

    auto root_index = tree.get_root_index();
    CHECK(tree.get_node(root_index) == root);

    auto mid_index = tree.index(root_index, 0);
    CHECK(tree.get_node(mid_index) == mid);

    auto leaf_index = tree.index(mid_index, 0);
    CHECK(tree.get_node(leaf_index) == leaf);

    SECTION("Invalid link") {
        auto new_zero = make_value<double>(0);
        tree.replace_index(mid_index, new_zero);
        CHECK_THROWS_AS(tree.get_node(leaf_index), InvalidIndexError);
    }
}

struct CountTraverser : public TreeTraverser {
    CountTraverser(NodeTree& tree) :
        TreeTraverser(tree)
    {}

    bool object_start(NodeTree::Index index) override {
        add_count(started, tree.get_node(index));
        return true;
    }
    void object_end(NodeTree::Index index) override {
        add_count(ended, tree.get_node(index));
    }

    using CountMap = map<AbstractReference, size_t>;

    void add_count(CountMap& map, AbstractReference node) {
        auto iter = map.find(node);
        if (iter == map.end()) {
            map.emplace(std::move(node), 1);
        } else {
            ++iter->second;
        }
    }

    CountMap started;
    CountMap ended;
};

TEST_CASE("Traverse node tree", "[node]") {
    auto root = make_shared<Add>();

    auto tree = NodeTree(root);

    SECTION("Empty") {
        CountTraverser traverser(tree);
        traverser.traverse_tree();

        CHECK(traverser.started.size() == 3);
        CHECK(traverser.ended.size() == 3);
        CHECK(traverser.started[root] == 1);
        CHECK(traverser.ended[root] == 1);
        CHECK(traverser.started[root->get_link(0)] == 1);
        CHECK(traverser.ended[root->get_link(1)] == 1);
    }

    SECTION("Same link") {
        tree.replace_index(tree.index(tree.get_root_index(), 1), root->get_link(0));

        CountTraverser traverser(tree);
        traverser.traverse_tree();

        CHECK(traverser.started.size() == 2);
        CHECK(traverser.ended[root] == 1);
        CHECK(traverser.started[root->get_link(0)] == 2);
        CHECK(traverser.ended[root->get_link(0)] == 2);
    }

    SECTION("Deeper tree & removal") {
        auto sub_add = make_shared<Add>();
        sub_add->set_link(0, root->get_link(0));
        tree.replace_index(tree.index(tree.get_root_index(), 1), sub_add);

        {
            CountTraverser traverser(tree);
            traverser.traverse_tree();

            CHECK(traverser.started.size() == 4);
            CHECK(traverser.started[root->get_link(0)] == 2);
            CHECK(traverser.ended[sub_add] == 1);
        }

        auto new_link = make_value<double>(0);
        tree.replace_index(tree.index(tree.get_root_index(), 1), new_link);

        {
            CountTraverser traverser(tree);
            traverser.traverse_tree();

            CHECK(traverser.started.size() == 3);
            CHECK(traverser.started[root->get_link(0)] == 1);
            CHECK(traverser.ended.find(sub_add) == traverser.ended.end());
        }
    }
}
