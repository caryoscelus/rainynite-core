/*  nodes/test/transform.cpp - node to test transform at index
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

#include <core/node_info.h>
#include <core/node/new_node.h>
#include <core/node/list.h>
#include <core/node_tree_transform.h>
#include <core/context.h>
#include <core/document.h>
#include <core/exceptions.h>

namespace rainynite::core::nodes {

class TestTransform :
    public NewNode<
        TestTransform,
        Geom::Affine,
        types::Only<vector<double>>
    >
{
    DOC_STRING(
        "Return transform at specific index.\n"
        "\n"
        "This node is primarily made for testing purposes."
    )

    NODE_PROPERTIES("index")
    static vector<AbstractReference> const& default_values() {
        static vector<AbstractReference> instance {
            make_node<ListValue<double>>()
        };
        return instance;
    }

    PROPERTY(index)

public:
    Geom::Affine get(shared_ptr<Context> ctx) const override {
        auto document = ctx->get_document();
        if (document == nullptr)
            throw NullPointerException("No document");
        auto tree = document->get_tree();

        NodeTreePath path;
        for (auto i : index_value<vector<double>>(ctx)) {
            path.indexes.push_back((size_t)std::fmax(i, 0));
        }

        auto idx = tree_path_to_index(*tree, path);

        return tree->get_element<TreeCalculateTransform>(idx)->get_transform(ctx);
    }

};

REGISTER_NODE(TestTransform);

} // namespace rainynite::core::nodes
