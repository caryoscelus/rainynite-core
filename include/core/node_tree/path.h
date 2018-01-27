/*  node_tree/path.h - Node tree path
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

#ifndef CORE_NODE_TREE_PATH_H_17A557AE_B48F_5C2A_AB42_99B184998CBC
#define CORE_NODE_TREE_PATH_H_17A557AE_B48F_5C2A_AB42_99B184998CBC

#include "node_tree.h"

namespace rainynite::core {

struct NodeTreePath {
    NodeTreePath(vector<size_t> const& idxs) :
        indexes(idxs)
    {}

    NodeTreePath() = default;

    vector<size_t> indexes;

    bool operator==(NodeTreePath const& other) const {
        return indexes == other.indexes;
    }
    bool operator!=(NodeTreePath const& other) const {
        return !(*this == other);
    }
};

NodeTree::Index tree_path_to_index(NodeTree const& tree, NodeTreePath const& path);
NodeTreePath tree_index_to_path(NodeTree const& tree, NodeTree::Index index);

AbstractReference get_node_by_path(NodeTree const& tree, NodeTreePath const& path);

} // namespace rainynite::core

#endif
