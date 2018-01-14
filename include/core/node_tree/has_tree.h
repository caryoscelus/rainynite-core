/*  node_tree/has_tree.h - HasTree
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

#ifndef CORE_NODE_TREE_HAS_TREE_H_8C08C191_BA54_5670_B7B1_7199099FB694
#define CORE_NODE_TREE_HAS_TREE_H_8C08C191_BA54_5670_B7B1_7199099FB694

#include <core/std/memory.h>
#include <core/util/nullptr.h>

namespace rainynite::core {

class NodeTree;

class HasTree {
public:
    HasTree(weak_ptr<NodeTree> tree_) :
        m_tree(tree_)
    {}
protected:
    shared_ptr<NodeTree> tree() const {
        return no_null(m_tree.lock());
    }
private:
    weak_ptr<NodeTree> m_tree;
};

} // namespace rainynite::core

#endif
