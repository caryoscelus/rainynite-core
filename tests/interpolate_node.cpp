/*  interpolate_node.cpp - node tree / interpolate frame tests
 *  Copyright (C) 2018 caryoscelus
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

#include <core/node_info/node_info.h>
#include <core/node_tree/actions.h>
#include <core/node_tree/traverse.h>
#include <core/node/abstract_node.h>
#include "zero_context.h"

using namespace rainynite;
using namespace rainynite::core;

class EmptyTraverser : public TreeTraverser {
public:
    EmptyTraverser(NodeTree& tree) :
        TreeTraverser(tree)
    {}
protected:
    bool object_start(NodeTree::Index /*index*/) override {
        return true;
    }
    void object_end(NodeTree::Index /*index*/) override {
    }
};

TEST_CASE("Interpolate / node tree / add frames", "[node]") {
    auto root = make_node_with_name<BaseValue<double>>("Interpolate/Real");
    auto root_node = abstract_node_cast(root);
    root_node->get_property("interpolate_with")->set_any(string{"WeightedAverage/Real"});

    CHECK(root->value(zero_context()) == 0.0);

    auto tree = NodeTree(root);

    root->set_any_at(4.5, zero_context());
    CHECK(root->value(zero_context()) == 4.5);

    CHECK(tree.get_node_count(root) == 1);
    auto frame_list = list_cast(root_node->get_property("keyframes"));

    CHECK(tree.get_node_count(frame_list->get_link(0)) == 1);

    EmptyTraverser traverser(tree);
    traverser.traverse_tree();
}
