/*  node_tree.cpp - node tree tests
 *  Copyright (C) 2017 caryoscelus
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

#include <core/node_tree_traverse.h>
#include "new_node.h"

using namespace rainynite;
using namespace rainynite::core;

struct CountTraverser : public TreeTraverser {
    CountTraverser(NodeTree& tree) :
        TreeTraverser(tree)
    {}

    bool object_start() override {
        add_count(started, current().node);
        return true;
    }
    void object_end() override {
        add_count(ended, current().node);
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

    auto tree = NodeTree(root, nullptr);

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
        root->set_link(1, root->get_link(0));

        CountTraverser traverser(tree);
        traverser.traverse_tree();

        CHECK(traverser.started.size() == 2);
        CHECK(traverser.ended[root] == 1);
        CHECK(traverser.started[root->get_link(0)] == 2);
        CHECK(traverser.ended[root->get_link(0)] == 2);
    }

    SECTION("Deeper tree") {
        auto sub_add = make_shared<Add>();
        sub_add->set_link(0, root->get_link(0));
        root->set_link(1, sub_add);

        CountTraverser traverser(tree);
        traverser.traverse_tree();

        CHECK(traverser.started.size() == 4);
        CHECK(traverser.started[root->get_link(0)] == 2);
        CHECK(traverser.ended[sub_add] == 1);
    }
}
