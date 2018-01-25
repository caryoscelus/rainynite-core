/*  node_tree/index.h - Node tree index & forward declaration
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

#ifndef CORE_NODE_TREE_INDEX_H_CD262A45_AF7E_5E1D_BE5A_5E85747BACB2
#define CORE_NODE_TREE_INDEX_H_CD262A45_AF7E_5E1D_BE5A_5E85747BACB2

#include <boost/operators.hpp>

namespace rainynite::core {

class NodeTree;

struct NodeTreeIndex : boost::totally_ordered<NodeTreeIndex> {
    NodeTreeIndex(size_t value_=0) :
        value(value_)
    {}
    bool operator==(NodeTreeIndex const& other) const {
        return value == other.value;
    }
    bool operator<(NodeTreeIndex const& other) const {
        return value < other.value;
    }
    NodeTreeIndex& operator++() {
        ++value;
        return *this;
    }
    NodeTreeIndex& operator--() {
        --value;
        return *this;
    }
    operator bool() const {
        return value;
    }
    operator size_t() const {
        return value;
    }
    size_t value;
};

} // namespace rainynite::core

#endif
