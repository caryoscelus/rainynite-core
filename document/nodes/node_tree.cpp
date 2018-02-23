/*  nodes/node_tree.cpp - node tree related nodes
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

#include <core/node_info/macros.h>
#include <core/node/new_node.h>
#include <core/node/list.h>
#include <core/node_tree/path.h>
#include <core/context.h>

namespace rainynite::core::nodes {

class NodeTreePathFromList :
    public NewNode<
        NodeTreePathFromList,
        NodeTreePath,
        types::Only<vector<double>>
    >
{
    DOC_STRING(
        "Convert list to node tree path"
    )

    NODE_PROPERTIES("source")
    COMPLEX_DEFAULT_VALUES(make_node<ListValue<double>>())
    PROPERTY(source)

protected:
    NodeTreePath get(shared_ptr<Context> ctx) const override {
        NodeTreePath path;
        auto indexes = source_value<vector<double>>(ctx);
        for (auto i : indexes) {
            path.indexes.push_back((size_t)std::fmax(i, 0));
        }
        return path;
    }
};

REGISTER_NODE(NodeTreePathFromList);

} // namespace rainynite::core::nodes
