/*  node_tree_module.cpp - node tree adding module tests
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

class CountDepth : TREE_ELEMENT(CountDepth) {
public:
    void added(NodeTree const& tree, NodeTree::Index index) override {
        auto p_idx = tree.parent(index);
        if (!p_idx->null()) {
            auto p_d = tree.get_element<CountDepth>(p_idx)->depth();
            depth_ = p_d + 1;
        } else {
            depth_ = 0;
        }
    }

    size_t depth() const {
        return depth_;
    }

private:
    size_t depth_;
};

TEST_CASE("Traverse node tree", "[node]") {
    auto root = make_shared<Add>();
    auto mid = make_shared<Add>();
    auto leaf = mid->get_link(0);
    root->set_link(0, mid);

    auto tree = NodeTree(root, nullptr);

    auto root_index = tree.get_root_index();
    auto mid_index = tree.index(root_index, 0);
    auto leaf_index = tree.index(mid_index, 0);

    CHECK(tree.get_element<CountDepth>(mid_index)->depth() == 1);

    CHECK(tree.get_element<CountDepth>(root_index)->depth() == 0);

    CHECK(tree.get_element<CountDepth>(leaf_index)->depth() == 2);
}
