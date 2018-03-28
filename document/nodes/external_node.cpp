/*  external_node.cpp - import node from external file
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
#include <core/node_info/default_node.h>
#include <core/node/new_node.h>
#include <core/node/proxy_node.h>
#include <core/node_tree/path.h>
#include <core/fs/document_loader.h>
#include <core/fs/from_context.h>
#include <core/util/nullptr.h>
#include <core/all_types.h>

namespace rainynite::core::nodes {

template <typename T>
class ExternalNode :
    public NewProxyNode<
        ExternalNode<T>,
        T,
        types::Only<fs::Path::path_t>,
        types::Only<NodeTreePath>
    >
{
    DOC_STRING(
        "Load a node from external file"
    )

    NODE_PROPERTIES("file_name", "node_path")
    COMPLEX_DEFAULT_VALUES(make_value<fs::Path::path_t>(), make_default_node<NodeTreePath>())
    PROPERTY(file_name)
    PROPERTY(node_path)

protected:
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        auto fname = file_name_value<fs::Path::path_t>(ctx);
        auto fpath = fs::context_path(*ctx, fname);
        auto doc = no_null(DocumentLoader::instance()->get_document(fpath));
        auto tree = doc->get_tree();
        auto npath = node_path_value<NodeTreePath>(ctx);
        auto idx = tree_path_to_index(*tree, npath);

        // TODO: ctx?..
        return {tree->get_node(idx), ctx};
    }
};

NODE_INFO_INSTANCES(ExternalNode, ExternalNode<T>, T)

} // namespace rainynite::core::nodes
